<?php
session_start();
require_once '../config/db.php';

header('Content-Type: application/json');

// Check if user is logged in
if (!isset($_SESSION['user_id'])) {
    http_response_code(401);
    echo json_encode(['success' => false, 'message' => 'Unauthorized']);
    exit();
}

// Get JSON input
$input = json_decode(file_get_contents('php://input'), true);
$id = filter_var($input['id'] ?? null, FILTER_VALIDATE_INT);

if (!$id) {
    http_response_code(400);
    echo json_encode(['success' => false, 'message' => 'Invalid news ID']);
    exit();
}

try {
    // Check if news exists and belongs to the user
    $stmt = $pdo->prepare("SELECT id FROM news WHERE id = ? AND user_id = ?");
    $stmt->execute([$id, $_SESSION['user_id']]);
    
    if (!$stmt->fetch()) {
        http_response_code(403);
        echo json_encode(['success' => false, 'message' => 'News not found or unauthorized']);
        exit();
    }

    // Delete the news
    $stmt = $pdo->prepare("DELETE FROM news WHERE id = ? AND user_id = ?");
    if ($stmt->execute([$id, $_SESSION['user_id']])) {
        echo json_encode(['success' => true]);
    } else {
        http_response_code(500);
        echo json_encode(['success' => false, 'message' => 'Failed to delete news']);
    }
} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
}
?> 