using MySql.Data.MySqlClient;
using TopicsAndPosts.Models;

namespace TopicsAndPosts.Data
{
    public class DatabaseService
    {
        private readonly string _connectionString;

        public DatabaseService(string connectionString)
        {
            _connectionString = connectionString;
            InitializeDatabase();
        }

        private void InitializeDatabase()
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            // Create Topics table
            var createTopicsTable = @"
                CREATE TABLE IF NOT EXISTS Topics (
                    Id INT AUTO_INCREMENT PRIMARY KEY,
                    TopicName VARCHAR(255) NOT NULL
                )";

            // Create Posts table
            var createPostsTable = @"
                CREATE TABLE IF NOT EXISTS Posts (
                    Id INT AUTO_INCREMENT PRIMARY KEY,
                    User VARCHAR(255) NOT NULL,
                    TopicId INT NOT NULL,
                    Text TEXT NOT NULL,
                    Date DATETIME NOT NULL,
                    FOREIGN KEY (TopicId) REFERENCES Topics (Id)
                )";

            using var command1 = new MySqlCommand(createTopicsTable, connection);
            command1.ExecuteNonQuery();

            using var command2 = new MySqlCommand(createPostsTable, connection);
            command2.ExecuteNonQuery();
        }

        public List<Topic> GetAllTopics()
        {
            var topics = new List<Topic>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = "SELECT Id, TopicName FROM Topics";
            using var command = new MySqlCommand(sql, connection);
            using var reader = command.ExecuteReader();

            while (reader.Read())
            {
                topics.Add(new Topic
                {
                    Id = reader.GetInt32(0),
                    TopicName = reader.GetString(1)
                });
            }

            return topics;
        }

        public Topic? GetTopicByName(string topicName)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = "SELECT Id, TopicName FROM Topics WHERE TopicName = @topicName";
            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@topicName", topicName);

            using var reader = command.ExecuteReader();
            if (reader.Read())
            {
                return new Topic
                {
                    Id = reader.GetInt32(0),
                    TopicName = reader.GetString(1)
                };
            }

            return null;
        }

        public int CreateTopic(string topicName)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = "INSERT INTO Topics (TopicName) VALUES (@topicName); SELECT LAST_INSERT_ID()";
            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@topicName", topicName);

            return Convert.ToInt32(command.ExecuteScalar());
        }

        public List<Post> GetPostsByTopic(int topicId)
        {
            var posts = new List<Post>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = @"
                SELECT p.Id, p.User, p.TopicId, p.Text, p.Date, t.TopicName 
                FROM Posts p 
                JOIN Topics t ON p.TopicId = t.Id 
                WHERE p.TopicId = @topicId 
                ORDER BY p.Date DESC";

            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@topicId", topicId);

            using var reader = command.ExecuteReader();
            while (reader.Read())
            {
                posts.Add(new Post
                {
                    Id = reader.GetInt32(0),
                    User = reader.GetString(1),
                    TopicId = reader.GetInt32(2),
                    Text = reader.GetString(3),
                    Date = reader.GetDateTime(4),
                    Topic = new Topic
                    {
                        Id = reader.GetInt32(2),
                        TopicName = reader.GetString(5)
                    }
                });
            }

            return posts;
        }

        public List<Post> GetAllPosts()
        {
            var posts = new List<Post>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = @"
                SELECT p.Id, p.User, p.TopicId, p.Text, p.Date, t.TopicName 
                FROM Posts p 
                JOIN Topics t ON p.TopicId = t.Id 
                ORDER BY p.Date DESC";

            using var command = new MySqlCommand(sql, connection);
            using var reader = command.ExecuteReader();

            while (reader.Read())
            {
                posts.Add(new Post
                {
                    Id = reader.GetInt32("Id"),
                    User = reader.GetString("User"),
                    TopicId = reader.GetInt32("TopicId"),
                    Text = reader.GetString("Text"),
                    Date = reader.GetDateTime("Date"),
                    Topic = new Topic
                    {
                        Id = reader.GetInt32("TopicId"),
                        TopicName = reader.GetString("TopicName")
                    }
                });
            }

            return posts;
        }

        public void CreatePost(string user, int topicId, string text)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = "INSERT INTO Posts (User, TopicId, Text, Date) VALUES (@user, @topicId, @text, @date)";
            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@user", user);
            command.Parameters.AddWithValue("@topicId", topicId);
            command.Parameters.AddWithValue("@text", text);
            command.Parameters.AddWithValue("@date", DateTime.Now);

            command.ExecuteNonQuery();
        }

        public void UpdatePost(int postId, string user, string text)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = "UPDATE Posts SET User = @user, Text = @text, Date = @date WHERE Id = @id";
            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@user", user);
            command.Parameters.AddWithValue("@text", text);
            command.Parameters.AddWithValue("@date", DateTime.Now);
            command.Parameters.AddWithValue("@id", postId);

            command.ExecuteNonQuery();
        }

        public Post? GetPost(int postId)
        {
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = @"
                SELECT p.Id, p.User, p.TopicId, p.Text, p.Date, t.TopicName 
                FROM Posts p 
                JOIN Topics t ON p.TopicId = t.Id 
                WHERE p.Id = @id";

            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@id", postId);

            using var reader = command.ExecuteReader();
            if (reader.Read())
            {
                return new Post
                {
                    Id = reader.GetInt32("Id"),
                    User = reader.GetString("User"),
                    TopicId = reader.GetInt32("TopicId"),
                    Text = reader.GetString("Text"),
                    Date = reader.GetDateTime("Date"),
                    Topic = new Topic
                    {
                        Id = reader.GetInt32("TopicId"),
                        TopicName = reader.GetString("TopicName")
                    }
                };
            }

            return null;
        }

        public List<Post> GetRecentPosts(DateTime since)
        {
            var posts = new List<Post>();
            using var connection = new MySqlConnection(_connectionString);
            connection.Open();

            var sql = @"
                SELECT p.Id, p.User, p.TopicId, p.Text, p.Date, t.TopicName 
                FROM Posts p 
                JOIN Topics t ON p.TopicId = t.Id 
                WHERE p.Date > @since 
                ORDER BY p.Date DESC";

            using var command = new MySqlCommand(sql, connection);
            command.Parameters.AddWithValue("@since", since);

            using var reader = command.ExecuteReader();
            while (reader.Read())
            {
                posts.Add(new Post
                {
                    Id = reader.GetInt32("Id"),
                    User = reader.GetString("User"),
                    TopicId = reader.GetInt32("TopicId"),
                    Text = reader.GetString("Text"),
                    Date = reader.GetDateTime("Date"),
                    Topic = new Topic
                    {
                        Id = reader.GetInt32("TopicId"),
                        TopicName = reader.GetString("TopicName")
                    }
                });
            }

            return posts;
        }
    }
}
