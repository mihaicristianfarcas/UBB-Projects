using MySql.Data.MySqlClient;
using HotelRoomReservation.Models;

namespace HotelRoomReservation.Data
{
    public class DatabaseService
    {
        private readonly string _connectionString;

        public DatabaseService(string connectionString)
        {
            _connectionString = connectionString;
        }

        public User? GetUserByUsernameAndPassword(string username, int password)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            using var command = new MySqlCommand("SELECT * FROM User WHERE username = @Username AND password = @Password", connection);
            command.Parameters.AddWithValue("@Username", username);
            command.Parameters.AddWithValue("@Password", password);

            using var reader = command.ExecuteReader();
            if (reader.Read())
            {
                return new User
                {
                    Id = reader.GetInt32("Id"),
                    Username = reader.GetString("Username"),
                    Password = reader.GetInt32("Password")
                };
            }

            return null;
        }

        public void AddUser(User user)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            using var command = new MySqlCommand("INSERT INTO User (username, password) VALUES (@Username, @Password)", connection);
            command.Parameters.AddWithValue("@Username", user.Username);
            command.Parameters.AddWithValue("@Password", user.Password);

            command.ExecuteNonQuery();
            user.Id = (int)command.LastInsertedId;
        }

        // Hotel Room methods
        public List<HotelRoom> GetAllHotelRooms()
        {
            var rooms = new List<HotelRoom>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            using var command = new MySqlCommand("SELECT * FROM HotelRoom", connection);
            using var reader = command.ExecuteReader();

            while (reader.Read())
            {
                rooms.Add(new HotelRoom
                {
                    Id = reader.GetInt32("Id"),
                    RoomNumber = reader.GetString("RoomNumber"),
                    Capacity = reader.GetInt32("Capacity"),
                    BasePrice = reader.GetInt32("BasePrice")
                });
            }

            return rooms;
        }

        public List<HotelRoom> GetAvailableRooms(DateTime checkIn, DateTime checkOut)
        {
            var rooms = new List<HotelRoom>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            string query = @"
                SELECT hr.* FROM HotelRoom hr
                WHERE hr.Id NOT IN (
                    SELECT DISTINCT r.roomId FROM Reservation r
                    WHERE (r.checkInDate < @CheckOut AND r.checkOutDate > @CheckIn)
                )";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@CheckIn", checkIn);
            command.Parameters.AddWithValue("@CheckOut", checkOut);

            using var reader = command.ExecuteReader();
            while (reader.Read())
            {
                rooms.Add(new HotelRoom
                {
                    Id = reader.GetInt32("Id"),
                    RoomNumber = reader.GetString("RoomNumber"),
                    Capacity = reader.GetInt32("Capacity"),
                    BasePrice = reader.GetInt32("BasePrice")
                });
            }

            return rooms;
        }

        public HotelRoom? GetHotelRoomById(int id)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            using var command = new MySqlCommand("SELECT * FROM HotelRoom WHERE Id = @Id", connection);
            command.Parameters.AddWithValue("@Id", id);

            using var reader = command.ExecuteReader();
            if (reader.Read())
            {
                return new HotelRoom
                {
                    Id = reader.GetInt32("Id"),
                    RoomNumber = reader.GetString("RoomNumber"),
                    Capacity = reader.GetInt32("Capacity"),
                    BasePrice = reader.GetInt32("BasePrice")
                };
            }

            return null;
        }

        // Reservation methods
        public int CalculateDynamicPrice(int roomId, DateTime checkIn, DateTime checkOut, int basePrice)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            // Get total number of rooms
            using var totalRoomsCommand = new MySqlCommand("SELECT COUNT(*) FROM HotelRoom", connection);
            int totalRooms = Convert.ToInt32(totalRoomsCommand.ExecuteScalar());

            // Count overlapping reservations for the date range
            string query = @"
                SELECT COUNT(DISTINCT roomId) FROM Reservation
                WHERE (checkInDate < @CheckOut AND checkOutDate > @CheckIn)";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@CheckIn", checkIn);
            command.Parameters.AddWithValue("@CheckOut", checkOut);

            int bookedRooms = Convert.ToInt32(command.ExecuteScalar());
            double occupancyRate = (double)bookedRooms / totalRooms;

            if (occupancyRate <= 0.5)
                return basePrice;
            else if (occupancyRate <= 0.8)
                return (int)(basePrice * 1.2);
            else
                return (int)(basePrice * 1.5);
        }

        public bool HasOverlappingReservation(int userId, DateTime checkIn, DateTime checkOut, int? excludeReservationId = null)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            string query = @"
                SELECT COUNT(*) FROM Reservation
                WHERE userId = @UserId 
                AND (checkInDate < @CheckOut AND checkOutDate > @CheckIn)";

            if (excludeReservationId.HasValue)
                query += " AND Id != @ExcludeId";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@UserId", userId);
            command.Parameters.AddWithValue("@CheckIn", checkIn);
            command.Parameters.AddWithValue("@CheckOut", checkOut);

            if (excludeReservationId.HasValue)
                command.Parameters.AddWithValue("@ExcludeId", excludeReservationId.Value);

            return Convert.ToInt32(command.ExecuteScalar()) > 0;
        }

        public void AddReservation(Reservation reservation)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            string query = @"
                INSERT INTO Reservation (userId, roomId, checkInDate, checkOutDate, numberOfGuests, totalPrice)
                VALUES (@UserId, @RoomId, @CheckInDate, @CheckOutDate, @NumberOfGuests, @TotalPrice)";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@UserId", reservation.UserId);
            command.Parameters.AddWithValue("@RoomId", reservation.RoomId);
            command.Parameters.AddWithValue("@CheckInDate", reservation.CheckInDate);
            command.Parameters.AddWithValue("@CheckOutDate", reservation.CheckOutDate);
            command.Parameters.AddWithValue("@NumberOfGuests", reservation.NumberOfGuests);
            command.Parameters.AddWithValue("@TotalPrice", reservation.TotalPrice);

            command.ExecuteNonQuery();
            reservation.Id = (int)command.LastInsertedId;
        }

        public List<Reservation> GetUserReservations(int userId)
        {
            var reservations = new List<Reservation>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            string query = @"
                SELECT r.*, hr.roomNumber, hr.basePrice
                FROM Reservation r
                JOIN HotelRoom hr ON r.roomId = hr.Id
                WHERE r.userId = @UserId
                ORDER BY r.checkInDate DESC";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@UserId", userId);

            using var reader = command.ExecuteReader();
            while (reader.Read())
            {
                reservations.Add(new Reservation
                {
                    Id = reader.GetInt32("Id"),
                    UserId = reader.GetInt32("UserId"),
                    RoomId = reader.GetInt32("RoomId"),
                    CheckInDate = reader.GetDateTime("CheckInDate"),
                    CheckOutDate = reader.GetDateTime("CheckOutDate"),
                    NumberOfGuests = reader.GetInt32("NumberOfGuests"),
                    TotalPrice = reader.GetInt32("TotalPrice"),
                    HotelRoom = new HotelRoom
                    {
                        Id = reader.GetInt32("RoomId"),
                        RoomNumber = reader.GetString("RoomNumber"),
                        BasePrice = reader.GetInt32("BasePrice")
                    }
                });
            }

            return reservations;
        }

        public int GetTotalGuestsForDate(DateTime date)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            string query = @"
                SELECT COALESCE(SUM(numberOfGuests), 0)
                FROM Reservation
                WHERE @Date >= checkInDate AND @Date < checkOutDate";

            using var command = new MySqlCommand(query, connection);
            command.Parameters.AddWithValue("@Date", date);

            return Convert.ToInt32(command.ExecuteScalar());
        }
    }
}
