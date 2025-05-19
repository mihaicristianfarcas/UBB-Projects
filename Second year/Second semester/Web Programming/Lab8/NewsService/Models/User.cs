using System;
using System.Collections.Generic;

namespace NewsService.Models
{
    public class User
    {
        public int Id { get; set; }
        public required string Username { get; set; }
        public required string Password { get; set; }
        public DateTime CreatedAt { get; set; }
        public ICollection<News>? News { get; set; }
    }
} 