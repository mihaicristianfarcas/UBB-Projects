<?php
session_start();
require_once '../config/db.php';

header('Content-Type: application/json');

try {
    $date_from = filter_input(INPUT_GET, 'date_from', FILTER_SANITIZE_STRING);
    $date_to = filter_input(INPUT_GET, 'date_to', FILTER_SANITIZE_STRING);
    $category = filter_input(INPUT_GET, 'category', FILTER_SANITIZE_STRING);

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

    $stmt = $pdo->prepare($query);
    $stmt->execute($params);
    $news = $stmt->fetchAll();

    // Add can_edit flag for each news item
    foreach ($news as &$item) {
        $item['can_edit'] = isset($_SESSION['user_id']) && $item['user_id'] == $_SESSION['user_id'];
    }

    echo json_encode($news);
} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['error' => 'Database error: ' . $e->getMessage()]);
}
?> 