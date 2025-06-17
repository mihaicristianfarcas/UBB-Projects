using System;
using System.Collections.Generic;

namespace ASPTest.Models
{
    public class User
    {
        public int Id { get; set; }
        public required string Username { get; set; }
        public required string Password { get; set; }
    }
}