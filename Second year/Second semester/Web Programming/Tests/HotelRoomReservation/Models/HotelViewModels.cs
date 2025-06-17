using HotelRoomReservation.Models;

namespace HotelRoomReservation.Models
{
    public class AvailableRoomsViewModel
    {
        public DateTime CheckInDate { get; set; }
        public DateTime CheckOutDate { get; set; }
        public List<HotelRoomWithPrice> AvailableRooms { get; set; } = new();
    }

    public class HotelRoomWithPrice : HotelRoom
    {
        public int DynamicPrice { get; set; }
    }

    public class GuestCountViewModel
    {
        public DateTime Date { get; set; }
        public int TotalGuests { get; set; }
    }
}
