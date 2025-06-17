using Microsoft.AspNetCore.Mvc;
using HotelRoomReservation.Models;
using HotelRoomReservation.Data;
using HotelRoomReservation.Extensions;
using HotelRoomReservation.Attributes;

namespace HotelRoomReservation.Controllers
{
    public class HotelController : BaseController
    {
        private readonly DatabaseService _databaseService;

        public HotelController(DatabaseService databaseService)
        {
            _databaseService = databaseService;
        }

        [HttpGet]
        [Authorize]
        public IActionResult AvailableRooms(DateTime? checkIn, DateTime? checkOut)
        {
            var model = new AvailableRoomsViewModel
            {
                CheckInDate = checkIn ?? DateTime.Today.AddDays(1),
                CheckOutDate = checkOut ?? DateTime.Today.AddDays(2)
            };

            // Validate dates
            if (model.CheckInDate >= model.CheckOutDate)
            {
                ModelState.AddModelError("", "Check-in date must be before check-out date");
                return View(model);
            }
            if (model.CheckInDate < DateTime.Today)
            {
                ModelState.AddModelError("", "Check-in date cannot be in the past");
                return View(model);
            }

            // Always load available rooms for the model's dates
            model.AvailableRooms = _databaseService.GetAvailableRooms(model.CheckInDate, model.CheckOutDate)
                .Select(room => new HotelRoomWithPrice
                {
                    Id = room.Id,
                    RoomNumber = room.RoomNumber,
                    Capacity = room.Capacity,
                    BasePrice = room.BasePrice,
                    DynamicPrice = _databaseService.CalculateDynamicPrice(room.Id, model.CheckInDate, model.CheckOutDate, room.BasePrice)
                }).ToList();

            return View(model);
        }

        [HttpPost]
        [Authorize]
        public IActionResult Reserve(int roomId, DateTime checkIn, DateTime checkOut, int numberOfGuests)
        {
            var userId = HttpContext.Session.GetUserId();

            // Basic validation
            if (checkIn >= checkOut)
            {
                TempData["Error"] = "Check-in date must be before check-out date";
                return RedirectToAction("AvailableRooms", new { checkIn, checkOut });
            }

            if (checkIn < DateTime.Today)
            {
                TempData["Error"] = "Check-in date cannot be in the past";
                return RedirectToAction("AvailableRooms", new { checkIn, checkOut });
            }

            // Check for overlapping reservations for the same user
            if (_databaseService.HasOverlappingReservation(userId, checkIn, checkOut))
            {
                TempData["Error"] = "You already have a reservation during this period";
                return RedirectToAction("AvailableRooms", new { checkIn, checkOut });
            }

            // Get room details
            var room = _databaseService.GetHotelRoomById(roomId);
            if (room == null)
            {
                TempData["Error"] = "Room not found";
                return RedirectToAction("AvailableRooms", new { checkIn, checkOut });
            }

            // Check room capacity
            if (numberOfGuests > room.Capacity)
            {
                TempData["Error"] = $"Room capacity is {room.Capacity} guests";
                return RedirectToAction("AvailableRooms", new { checkIn, checkOut });
            }

            // Calculate total price
            int dynamicPrice = _databaseService.CalculateDynamicPrice(roomId, checkIn, checkOut, room.BasePrice);
            int nights = (checkOut - checkIn).Days;
            int totalPrice = dynamicPrice * nights;

            // Create reservation
            var reservation = new Reservation
            {
                UserId = userId,
                RoomId = roomId,
                CheckInDate = checkIn,
                CheckOutDate = checkOut,
                NumberOfGuests = numberOfGuests,
                TotalPrice = totalPrice
            };

            _databaseService.AddReservation(reservation);

            TempData["Success"] = $"Room {room.RoomNumber} reserved successfully! Total: ${totalPrice}";
            return RedirectToAction("MyReservations");
        }

        [HttpGet]
        [Authorize]
        public IActionResult MyReservations()
        {
            var userId = HttpContext.Session.GetUserId();
            var reservations = _databaseService.GetUserReservations(userId);

            return View(reservations);
        }

        [HttpGet]
        [Authorize]
        public IActionResult GuestCount(DateTime? date)
        {
            var model = new GuestCountViewModel
            {
                Date = date ?? DateTime.Today
            };

            model.TotalGuests = _databaseService.GetTotalGuestsForDate(model.Date);

            return View(model);
        }
    }
}
