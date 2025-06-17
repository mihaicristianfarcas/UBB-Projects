using System.ComponentModel.DataAnnotations;

namespace HotelRoomReservation.Models
{
    public class HotelRoom
    {
        public int Id { get; set; }

        [Required]
        public required string RoomNumber { get; set; }

        [Required]
        [Range(1, 10)]
        public int Capacity { get; set; }

        [Required]
        [Range(1, int.MaxValue)]
        public int BasePrice { get; set; }
    }
}
