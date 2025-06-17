using Microsoft.AspNetCore.Mvc;
using HotelRoomReservation.Models;
using HotelRoomReservation.Data;
using HotelRoomReservation.Attributes;

namespace HotelRoomReservation.Controllers
{
    public class AccountController : Controller
    {
        private readonly DatabaseService _database;

        public AccountController(DatabaseService database)
        {
            _database = database;
        }

        public IActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Login(string username, int password)
        {
            var user = _database.GetUserByUsernameAndPassword(username, password);

            if (user != null)
            {
                HttpContext.Session.SetInt32("UserId", user.Id);
                HttpContext.Session.SetString("Username", user.Username);
                return RedirectToAction("Index", "Home");
            }

            ModelState.AddModelError("", "Invalid username or password");
            return View();
        }

        public IActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Register(string username, int password)
        {
            if (_database.GetUserByUsernameAndPassword(username, password) != null)
            {
                ModelState.AddModelError("Username", "Username already exists");
                return View();
            }

            var user = new User
            {
                Username = username,
                Password = password,
            };

            _database.AddUser(user);

            return RedirectToAction("Login");
        }

        [Authorize]
        public IActionResult Logout()
        {
            HttpContext.Session.Clear();
            return RedirectToAction("Login");
        }
    }
}