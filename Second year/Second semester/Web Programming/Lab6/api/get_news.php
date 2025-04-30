<?php
session_start();
require_once '../config/db.php';

header('Content-Type: application/json');

// Enable error reporting
error_reporting(E_ALL);
ini_set('display_errors', 1);

try {
    // Log the request
    error_log("GET request received with parameters: " . print_r($_GET, true));
    
    // Validate and sanitize input parameters
    $date_from = filter_input(INPUT_GET, 'date_from');
    $date_to = filter_input(INPUT_GET, 'date_to');
    $category = filter_input(INPUT_GET, 'category');

    // Log the processed parameters
    error_log("Processed parameters - date_from: $date_from, date_to: $date_to, category: $category");

    // Validate date format if provided
    if ($date_from && !strtotime($date_from)) {
        throw new Exception('Invalid date_from format');
    }
    if ($date_to && !strtotime($date_to)) {
        throw new Exception('Invalid date_to format');
    }

    // Build the query
    $query = "SELECT n.*, u.username as producer 
              FROM news n 
              JOIN users u ON n.user_id = u.id 
              WHERE 1=1";
    $params = [];

    if ($date_from) {
        $query .= " AND DATE(n.created_at) >= ?";
        $params[] = $date_from;
    }

    if ($date_to) {
        $query .= " AND DATE(n.created_at) <= ?";
        $params[] = $date_to;
    }

    if ($category) {
        $query .= " AND n.category = ?";
        $params[] = $category;
    }

    $query .= " ORDER BY n.created_at DESC";

    // Log the query and parameters
    error_log("Executing query: $query");
    error_log("With parameters: " . print_r($params, true));

    // Execute the query
    $stmt = $pdo->prepare($query);
    $stmt->execute($params);
    $news = $stmt->fetchAll();

    // Log the results
    error_log("Query returned " . count($news) . " results");

    // Add can_edit flag for each news item
    foreach ($news as &$item) {
        $item['can_edit'] = isset($_SESSION['user_id']) && $item['user_id'] == $_SESSION['user_id'];
    }

    // Return success response
    echo json_encode($news);

} catch (Exception $e) {
    error_log("Error in get_news.php: " . $e->getMessage());
    http_response_code(400);
    echo json_encode([
        'success' => false,
        'error' => $e->getMessage()
    ]);
} catch (PDOException $e) {
    error_log("Database error in get_news.php: " . $e->getMessage());
    http_response_code(500);
    echo json_encode([
        'success' => false,
        'error' => 'Database error occurred'
    ]);
}
?> 