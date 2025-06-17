using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using TopicsAndPosts.Models;
using TopicsAndPosts.Data;

namespace TopicsAndPosts.Controllers;

public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    private readonly DatabaseService _database;

    public HomeController(ILogger<HomeController> logger, DatabaseService database)
    {
        _logger = logger;
        _database = database;
    }

    public IActionResult Index()
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return View("SetUsername");
        }

        var lastCheckTime = HttpContext.Session.GetString("LastCheckTime");
        var lastCheck = string.IsNullOrEmpty(lastCheckTime) ? DateTime.MinValue : DateTime.Parse(lastCheckTime);

        var model = new HomeViewModel
        {
            CurrentUser = currentUser,
            Posts = _database.GetAllPosts(),
            Topics = _database.GetAllTopics(),
            RecentPosts = _database.GetRecentPosts(lastCheck)
        };

        // Update last check time
        HttpContext.Session.SetString("LastCheckTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));

        return View(model);
    }

    [HttpPost]
    public IActionResult SetUsername(string username)
    {
        if (!string.IsNullOrEmpty(username))
        {
            HttpContext.Session.SetString("CurrentUser", username);
            HttpContext.Session.SetString("LastCheckTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
        }
        return RedirectToAction("Index");
    }

    public IActionResult CreatePost()
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return RedirectToAction("Index");
        }

        return View();
    }

    [HttpPost]
    public IActionResult CreatePost(CreatePostViewModel model)
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return RedirectToAction("Index");
        }

        if (ModelState.IsValid)
        {
            // Check if topic exists, if not create it
            var topic = _database.GetTopicByName(model.TopicName);
            int topicId;

            if (topic == null)
            {
                topicId = _database.CreateTopic(model.TopicName);
            }
            else
            {
                topicId = topic.Id;
            }

            _database.CreatePost(currentUser, topicId, model.Text);
            return RedirectToAction("Index");
        }

        return View(model);
    }

    public IActionResult EditPost(int id)
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return RedirectToAction("Index");
        }

        var post = _database.GetPost(id);
        if (post == null)
        {
            return NotFound();
        }

        if (post.User != currentUser)
        {
            return RedirectToAction("Index");
        }

        var model = new EditPostViewModel
        {
            Id = post.Id,
            Text = post.Text,
            TopicName = post.Topic?.TopicName ?? ""
        };

        return View(model);
    }

    [HttpPost]
    public IActionResult EditPost(EditPostViewModel model)
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return RedirectToAction("Index");
        }

        if (ModelState.IsValid)
        {
            _database.UpdatePost(model.Id, currentUser, model.Text);
            return RedirectToAction("Index");
        }

        return View(model);
    }

    public IActionResult TopicPosts(int topicId)
    {
        var currentUser = HttpContext.Session.GetString("CurrentUser");
        if (string.IsNullOrEmpty(currentUser))
        {
            return RedirectToAction("Index");
        }

        var posts = _database.GetPostsByTopic(topicId);
        return View(posts);
    }

    public IActionResult Logout()
    {
        HttpContext.Session.Clear();
        return RedirectToAction("Index");
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
