<?php
session_start();
require_once 'config/db.php';

// Check if user is logged in
if (!isset($_SESSION['user_id'])) {
    header('Location: login.php');
    exit();
}

$errors = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = filter_input(INPUT_POST, 'title', FILTER_SANITIZE_STRING);
    $content = filter_input(INPUT_POST, 'content', FILTER_SANITIZE_STRING);
    $category = filter_input(INPUT_POST, 'category', FILTER_SANITIZE_STRING);

    if (empty($title)) {
        $errors[] = "Title is required";
    }
    if (empty($content)) {
        $errors[] = "Content is required";
    }
    if (empty($category)) {
        $errors[] = "Category is required";
    }

    if (empty($errors)) {
        try {
            $stmt = $pdo->prepare("INSERT INTO news (title, content, category, user_id) VALUES (?, ?, ?, ?)");
            if ($stmt->execute([$title, $content, $category, $_SESSION['user_id']])) {
                header('Location: index.php');
                exit();
            } else {
                $errors[] = "Failed to add news";
            }
        } catch (PDOException $e) {
            $errors[] = "Database error: " . $e->getMessage();
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Add News - News Service</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <h1>Add News</h1>
        
        <?php if (!empty($errors)): ?>
            <div class="error">
                <?php foreach ($errors as $error): ?>
                    <p><?php echo htmlspecialchars($error); ?></p>
                <?php endforeach; ?>
            </div>
        <?php endif; ?>

        <form method="POST" action="add_news.php">
            <div class="form-group">
                <label for="title">Title:</label>
                <input type="text" id="title" name="title" required>
            </div>
            
            <div class="form-group">
                <label for="category">Category:</label>
                <select id="category" name="category" required>
                    <option value="">Select a category</option>
                    <option value="politics">Politics</option>
                    <option value="society">Society</option>
                    <option value="health">Health</option>
                    <option value="technology">Technology</option>
                    <option value="sports">Sports</option>
                    <option value="entertainment">Entertainment</option>
                </select>
            </div>
            
            <div class="form-group">
                <label for="content">Content:</label>
                <textarea id="content" name="content" rows="10" required></textarea>
            </div>
            
            <div class="form-actions">
                <button type="submit" class="button">Add News</button>
                <a href="index.php" class="button">Cancel</a>
            </div>
        </form>
    </div>
</body>
</html> 