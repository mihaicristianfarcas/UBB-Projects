using System.ComponentModel.DataAnnotations;

namespace HotelRoomReservation.Models
{
    public class Reservation
    {
        public int Id { get; set; }

        [Required]
        public int UserId { get; set; }

        [Required]
        public int RoomId { get; set; }

        [Required]
        [Display(Name = "Check-in Date")]
        public DateTime CheckInDate { get; set; }

        [Required]
        [Display(Name = "Check-out Date")]
        public DateTime CheckOutDate { get; set; }

        [Required]
        [Range(1, 10)]
        [Display(Name = "Number of Guests")]
        public int NumberOfGuests { get; set; }

        public int TotalPrice { get; set; }

        // Navigation properties
        public User? User { get; set; }
        public HotelRoom? HotelRoom { get; set; }
    }
}
