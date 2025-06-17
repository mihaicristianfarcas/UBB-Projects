using Microsoft.AspNetCore.Mvc;
using ASPTest.Models;
using ASPTest.Data;
using ASPTest.Attributes;

namespace ASPTest.Controllers
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
        public IActionResult Login(string username, string password)
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
        public IActionResult Register(string username, string password)
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