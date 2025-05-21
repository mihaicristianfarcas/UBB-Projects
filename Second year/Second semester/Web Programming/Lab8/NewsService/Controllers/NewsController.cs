using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using NewsService.Data;
using NewsService.Models;
using NewsService.Extensions;

namespace NewsService.Controllers
{
    public class NewsController : BaseController
    {
        private readonly ApplicationDbContext _context;
        private const string PreviousFiltersKey = "PreviousFilters";
        private const int MaxPreviousFilters = 5;

        public NewsController(ApplicationDbContext context)
        {
            _context = context;
        }

        private void SaveFilterToHistory(NewsFilter filter)
        {
            var filters = HttpContext.Session.GetObject<List<NewsFilter>>(PreviousFiltersKey) ?? new List<NewsFilter>();
            
            // Add new filter if it's not already in the list
            if (!filters.Any(f => 
                f.StartDate == filter.StartDate && 
                f.EndDate == filter.EndDate && 
                f.Category == filter.Category))
            {
                filters.Add(filter);
                // Keep only the last MaxPreviousFilters filters
                if (filters.Count > MaxPreviousFilters)
                {
                    filters.RemoveAt(0);
                }
                HttpContext.Session.SetObject(PreviousFiltersKey, filters);
            }
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

            // Save filter to history if any filter is applied
            if (startDate.HasValue || endDate.HasValue || !string.IsNullOrEmpty(category))
            {
                var filter = new NewsFilter
                {
                    StartDate = startDate,
                    EndDate = endDate,
                    Category = category
                };
                SaveFilterToHistory(filter);
            }

            ViewBag.StartDate = startDate;
            ViewBag.EndDate = endDate;
            ViewBag.Category = category;
            ViewBag.Categories = await _context.News.Select(n => n.Category).Distinct().ToListAsync();
            ViewBag.PreviousFilters = HttpContext.Session.GetObject<List<NewsFilter>>(PreviousFiltersKey);

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