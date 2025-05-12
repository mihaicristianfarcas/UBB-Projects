<?php
session_start();

header('Access-Control-Allow-Origin: http://localhost:4200');
header('Access-Control-Allow-Credentials: true');
header('Access-Control-Allow-Methods: GET, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');

// Handle OPTIONS preflight request
if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit();
}

// Set content type to JSON for API requests
$isApi = strpos($_SERVER['HTTP_ACCEPT'] ?? '', 'application/json') !== false;

// Clear session
$_SESSION = array();
session_destroy();

if ($isApi) {
    header('Content-Type: application/json');
    echo json_encode(['success' => true]);
} else {
    header('Location: login.php');
}
exit();
?> 