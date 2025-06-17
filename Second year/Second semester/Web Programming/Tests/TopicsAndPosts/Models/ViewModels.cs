using System.ComponentModel.DataAnnotations;

namespace TopicsAndPosts.Models
{
    public class CreatePostViewModel
    {
        [Required]
        public string TopicName { get; set; } = string.Empty;

        [Required]
        public string Text { get; set; } = string.Empty;
    }

    public class EditPostViewModel
    {
        public int Id { get; set; }

        [Required]
        public string Text { get; set; } = string.Empty;

        public string TopicName { get; set; } = string.Empty;
    }

    public class HomeViewModel
    {
        public string CurrentUser { get; set; } = string.Empty;
        public List<Post> Posts { get; set; } = new();
        public List<Topic> Topics { get; set; } = new();
        public List<Post> RecentPosts { get; set; } = new();
    }
}
