namespace TopicsAndPosts.Models
{
    public class Post
    {
        public int Id { get; set; }
        public string User { get; set; } = string.Empty;
        public int TopicId { get; set; }
        public string Text { get; set; } = string.Empty;
        public DateTime Date { get; set; }

        public Topic? Topic { get; set; }
    }
}
