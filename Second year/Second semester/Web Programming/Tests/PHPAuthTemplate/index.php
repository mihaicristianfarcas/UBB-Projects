<?php
session_start();
require_once 'config/db.php';

// Get all categories for the filter
$stmt = $pdo->query("SELECT DISTINCT category FROM news ORDER BY category");
$categories = $stmt->fetchAll(PDO::FETCH_COLUMN);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>News Service</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <header>
            <h1>News Service</h1>
            <?php if (isset($_SESSION['user_id'])): ?>
                <div class="user-actions">
                    <a href="add_news.php" class="button">Add News</a>
                    <a href="logout.php" class="button">Logout</a>
                </div>
            <?php else: ?>
                <div class="user-actions">
                    <a href="login.php" class="button">Login</a>
                    <a href="register.php" class="button">Register</a>
                </div>
            <?php endif; ?>
        </header>

        <div class="filters">
            <div class="filter-group">
                <label for="date_from">From:</label>
                <input type="date" id="date_from">
            </div>
            <div class="filter-group">
                <label for="date_to">To:</label>
                <input type="date" id="date_to">
            </div>
            <div class="filter-group">
                <label for="category">Category:</label>
                <select id="category">
                    <option value="">All Categories</option>
                    <?php foreach ($categories as $category): ?>
                        <option value="<?php echo htmlspecialchars($category); ?>">
                            <?php echo htmlspecialchars($category); ?>
                        </option>
                    <?php endforeach; ?>
                </select>
            </div>
            <button id="apply_filters">Apply Filters</button>
        </div>

        <div id="active_filters" class="active-filters"></div>

        <div id="news_list" class="news-list">
            <!-- News items will be loaded here via AJAX -->
        </div>
    </div>

    <script src="js/news.js"></script>
</body>
</html> 