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

// Check if user is logged in
if (!isset($_SESSION['user_id'])) {
    if (strpos($_SERVER['HTTP_ACCEPT'] ?? '', 'application/json') !== false) {
        header('Content-Type: application/json');
        http_response_code(401);
        echo json_encode(['success' => false, 'message' => 'Unauthorized']);
        exit();
    } else {
        header('Location: login.php');
        exit();
    }
}

$errors = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Set content type to JSON for API requests
    if (strpos($_SERVER['HTTP_ACCEPT'] ?? '', 'application/json') !== false || 
        strpos($_SERVER['CONTENT_TYPE'] ?? '', 'application/json') !== false) {
        header('Content-Type: application/json');
        $isApi = true;
    } else {
        $isApi = false;
    }

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
            $stmt = $pdo->prepare("INSERT INTO news (title, content, category, importance, user_id) VALUES (?, ?, ?, ?, ?)");
            if ($stmt->execute([$title, $content, $category, $importance, $_SESSION['user_id']])) {
                if ($isApi) {
                    echo json_encode(['success' => true, 'id' => $pdo->lastInsertId()]);
                    exit();
                } else {
                    header('Location: index.php');
                    exit();
                }
            } else {
                $errors[] = "Failed to add news";
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

// Only show HTML for browser requests
if ($_SERVER['REQUEST_METHOD'] !== 'POST' || !isset($isApi) || !$isApi) {
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
                <label for="importance">Importance</label>
                <input type="text" id="importance" name="importance" required>
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
<?php } ?> 