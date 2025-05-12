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

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Set content type to JSON for API requests
    if (strpos($_SERVER['HTTP_ACCEPT'] ?? '', 'application/json') !== false || 
        strpos($_SERVER['CONTENT_TYPE'] ?? '', 'application/json') !== false) {
        header('Content-Type: application/json');
        $isApi = true;
    } else {
        $isApi = false;
    }

    $username = filter_input(INPUT_POST, 'username');
    $password = $_POST['password'] ?? '';

    $stmt = $pdo->prepare("SELECT id, username, password FROM users WHERE username = ?");
    $stmt->execute([$username]);
    $user = $stmt->fetch();

    if ($user && password_verify($password, $user['password'])) {
        $_SESSION['user_id'] = $user['id'];
        $_SESSION['username'] = $user['username'];
        
        if ($isApi) {
            echo json_encode([
                'success' => true,
                'user' => [
                    'id' => $user['id'],
                    'username' => $user['username']
                ]
            ]);
            exit();
        } else {
            header('Location: index.php');
            exit();
        }
    } else {
        $error = "Invalid username or password";
        if ($isApi) {
            http_response_code(401);
            echo json_encode(['success' => false, 'message' => $error]);
            exit();
        }
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
    <title>News Service - Login</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <h1>News Service Login</h1>
        <?php if (isset($error)): ?>
            <div class="error"><?php echo htmlspecialchars($error); ?></div>
        <?php endif; ?>
        <form method="POST" action="login.php">
            <div class="form-group">
                <label for="username">Username:</label>
                <input type="text" id="username" name="username" required>
            </div>
            <div class="form-group">
                <label for="password">Password:</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button type="submit">Login</button>
        </form>
        <p>Don't have an account? <a href="register.php">Register here</a></p>
    </div>
</body>
</html>
<?php } ?> 