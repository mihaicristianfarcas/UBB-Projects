using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;

namespace NewsService.Controllers
{
    public class BaseController : Controller
    {
        public override void OnActionExecuting(ActionExecutingContext context)
        {
            base.OnActionExecuting(context);

            // Skip authentication check for Account controller actions
            if (context.Controller.GetType().Name == "AccountController")
                return;

            // Check if user is logged in
            if (HttpContext.Session.GetInt32("UserId") == null)
            {
                context.Result = RedirectToAction("Login", "Account");
            }
        }
    }
} 