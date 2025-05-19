using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using NewsService.Data;
using NewsService.Models;

namespace NewsService.Controllers
{
    public class NewsController : BaseController
    {
        private readonly ApplicationDbContext _context;

        public NewsController(ApplicationDbContext context)
        {
            _context = context;
        }

        public async Task<IActionResult> Index(DateTime? startDate, DateTime? endDate, string category)
        {
            var query = _context.News.Include(n => n.User).AsQueryable();

            if (startDate.HasValue)
                query = query.Where(n => n.CreatedAt >= startDate.Value);

            if (endDate.HasValue)
                query = query.Where(n => n.CreatedAt <= endDate.Value);

            if (!string.IsNullOrEmpty(category))
                query = query.Where(n => n.Category == category);

            ViewBag.StartDate = startDate;
            ViewBag.EndDate = endDate;
            ViewBag.Category = category;
            ViewBag.Categories = await _context.News.Select(n => n.Category).Distinct().ToListAsync();

            return View(await query.OrderByDescending(n => n.CreatedAt).ToListAsync());
        }

        public IActionResult Create()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> Create(News news)
        {
            if (ModelState.IsValid)
            {
                var userId = HttpContext.Session.GetInt32("UserId") ?? throw new InvalidOperationException("User ID not found in session");
                news.UserId = userId;
                news.CreatedAt = DateTime.UtcNow;
                news.UpdatedAt = DateTime.UtcNow;

                _context.News.Add(news);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(news);
        }

        public async Task<IActionResult> Edit(int id)
        {
            var news = await _context.News.FindAsync(id);
            if (news == null || news.UserId != HttpContext.Session.GetInt32("UserId"))
            {
                return NotFound();
            }
            return View(news);
        }

        [HttpPost]
        public async Task<IActionResult> Edit(int id, News news)
        {
            if (id != news.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    var existingNews = await _context.News.FindAsync(id);
                    if (existingNews == null || existingNews.UserId != HttpContext.Session.GetInt32("UserId"))
                    {
                        return NotFound();
                    }

                    existingNews.Title = news.Title;
                    existingNews.Content = news.Content;
                    existingNews.Category = news.Category;
                    existingNews.Importance = news.Importance;
                    existingNews.UpdatedAt = DateTime.UtcNow;

                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!NewsExists(news.Id))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            return View(news);
        }

        [HttpPost]
        public async Task<IActionResult> Delete(int id)
        {
            var news = await _context.News.FindAsync(id);
            if (news == null || news.UserId != HttpContext.Session.GetInt32("UserId"))
            {
                return NotFound();
            }

            _context.News.Remove(news);
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool NewsExists(int id)
        {
            return _context.News.Any(e => e.Id == id);
        }
    }
} 