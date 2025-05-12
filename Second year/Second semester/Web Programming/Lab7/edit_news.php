<?php
session_start();
require_once 'config/db.php';

header('Access-Control-Allow-Origin: http://localhost:4200');
header('Access-Control-Allow-Credentials: true');
header('Access-Control-Allow-Methods: POST, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');

// Handle OPTIONS preflight request
if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit();
}

// Set content type to JSON for API requests
if (strpos($_SERVER['HTTP_ACCEPT'] ?? '', 'application/json') !== false || 
    strpos($_SERVER['CONTENT_TYPE'] ?? '', 'application/json') !== false) {
    header('Content-Type: application/json');
    $isApi = true;
} else {
    $isApi = false;
}

// Check if user is logged in
if (!isset($_SESSION['user_id'])) {
    if ($isApi) {
        http_response_code(401);
        echo json_encode(['success' => false, 'message' => 'Unauthorized']);
        exit();
    } else {
        header('Location: login.php');
        exit();
    }
}

$errors = [];
$news = null;

// Get news ID from URL
$id = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT);

if (!$id) {
    if ($isApi) {
        http_response_code(400);
        echo json_encode(['success' => false, 'message' => 'Invalid news ID']);
        exit();
    } else {
        header('Location: index.php');
        exit();
    }
}

// Get news item
$stmt = $pdo->prepare("SELECT * FROM news WHERE id = ? AND user_id = ?");
$stmt->execute([$id, $_SESSION['user_id']]);
$news = $stmt->fetch();

if (!$news) {
    if ($isApi) {
        http_response_code(404);
        echo json_encode(['success' => false, 'message' => 'News not found or you do not have permission']);
        exit();
    } else {
        header('Location: index.php');
        exit();
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = filter_input(INPUT_POST, 'title');
    $content = filter_input(INPUT_POST, 'content');
    $category = filter_input(INPUT_POST, 'category');
    $importance = filter_input(INPUT_POST, 'importance');

    if (empty($title)) {
        $errors[] = "Title is required";
    }
    if (empty($content)) {
        $errors[] = "Content is required";
    }
    if (empty($category)) {
        $errors[] = "Category is required";
    }
    if (empty($importance)) {
        $errors[] = "Importance is required";
    }

    if (empty($errors)) {
        try {
            $stmt = $pdo->prepare("UPDATE news SET title = ?, content = ?, category = ?, importance = ? WHERE id = ? AND user_id = ?");
            if ($stmt->execute([$title, $content, $category, $importance, $id, $_SESSION['user_id']])) {
                if ($isApi) {
                    echo json_encode(['success' => true]);
                    exit();
                } else {
                    header('Location: index.php');
                    exit();
                }
            } else {
                $errors[] = "Failed to update news";
            }
        } catch (PDOException $e) {
            $errors[] = "Database error: " . $e->getMessage();
        }
    }
    
    if (!empty($errors) && $isApi) {
        http_response_code(400);
        echo json_encode(['success' => false, 'errors' => $errors]);
        exit();
    }
}

// For API GET requests, return the news item
if ($_SERVER['REQUEST_METHOD'] === 'GET' && $isApi) {
    echo json_encode(['success' => true, 'news' => $news]);
    exit();
}

// Only show HTML for browser requests
if (!$isApi) {
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Edit News - News Service</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <h1>Edit News</h1>
        
        <?php if (!empty($errors)): ?>
            <div class="error">
                <?php foreach ($errors as $error): ?>
                    <p><?php echo htmlspecialchars($error); ?></p>
                <?php endforeach; ?>
            </div>
        <?php endif; ?>

        <form method="POST" action="edit_news.php?id=<?php echo $id; ?>">
            <div class="form-group">
                <label for="title">Title:</label>
                <input type="text" id="title" name="title" value="<?php echo htmlspecialchars($news['title']); ?>" required>
            </div>
            
            <div class="form-group">
                <label for="category">Category:</label>
                <select id="category" name="category" required>
                    <option value="">Select a category</option>
                    <option value="politics" <?php echo $news['category'] === 'politics' ? 'selected' : ''; ?>>Politics</option>
                    <option value="society" <?php echo $news['category'] === 'society' ? 'selected' : ''; ?>>Society</option>
                    <option value="health" <?php echo $news['category'] === 'health' ? 'selected' : ''; ?>>Health</option>
                    <option value="technology" <?php echo $news['category'] === 'technology' ? 'selected' : ''; ?>>Technology</option>
                    <option value="sports" <?php echo $news['category'] === 'sports' ? 'selected' : ''; ?>>Sports</option>
                    <option value="entertainment" <?php echo $news['category'] === 'entertainment' ? 'selected' : ''; ?>>Entertainment</option>
                </select>
            </div>

            <div class="form-group">
                <label for="importance">Importance</label>
                <input type="text" id="importance" name="importance" value="<?php echo htmlspecialchars($news['importance']); ?>" required>
            </div>
            
            <div class="form-group">
                <label for="content">Content:</label>
                <textarea id="content" name="content" rows="10" required><?php echo htmlspecialchars($news['content']); ?></textarea>
            </div>
            
            <div class="form-actions">
                <button type="submit" class="button">Update News</button>
                <a href="index.php" class="button">Cancel</a>
            </div>
        </form>
    </div>
</body>
</html>
<?php } ?> 