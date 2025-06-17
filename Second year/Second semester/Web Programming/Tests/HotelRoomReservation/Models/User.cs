using System;
using System.Collections.Generic;

namespace HotelRoomReservation.Models
{
    public class User
    {
        public int Id { get; set; }
        public required string Username { get; set; }
        public required int Password { get; set; }
    }
}