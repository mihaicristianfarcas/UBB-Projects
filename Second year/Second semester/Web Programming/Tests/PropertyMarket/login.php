<?php
session_start();
require_once 'config/db.php';

$error = '';
$showSecretQuestion = false;
$userDetails = null;
$userName = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['step']) && $_POST['step'] === 'get_question') {

        $name = trim(filter_input(INPUT_POST, 'name'));
        
        if (!empty($name)) {
            $stmt = $pdo->prepare("SELECT id, name, secretQuestion FROM user WHERE name = ?");
            $stmt->execute([$name]);
            $userDetails = $stmt->fetch();
            
            if ($userDetails) {
                $showSecretQuestion = true;
                $userName = $userDetails['name'];
            } else {
                $error = "User not found. Please check your name.";
            }
        } else {
            $error = "Please enter your name.";
        }
    } elseif (isset($_POST['step']) && $_POST['step'] === 'login') {

        $name = trim(filter_input(INPUT_POST, 'name'));
        $secretAnswer = trim(filter_input(INPUT_POST, 'secretAnswer'));
        
        if (!empty($name) && !empty($secretAnswer)) {
            $stmt = $pdo->prepare("SELECT id, name, secretQuestion, secretAnswer FROM user WHERE name = ?");
            $stmt->execute([$name]);
            $userDetails = $stmt->fetch();
            
            if ($userDetails) {
                if ($userDetails['secretAnswer'] === $secretAnswer) {
                    // Authentication successful
                    $_SESSION['user_id'] = $userDetails['id'];
                    $_SESSION['name'] = $userDetails['name'];
                    header("Location: index.php");
                    exit();
                } else {
                    $error = "Incorrect secret answer. Please try again.";
                    $showSecretQuestion = true;
                    $userName = $userDetails['name'];
                }
            } else {
                $error = "User not found.";
            }
        } else {
            $error = "Please provide both name and secret answer.";
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Login</title>
        <link rel="stylesheet" href="css/style.css">
    </head>
    <body>
        <div class="container">
            <h1>Login</h1>
            <p>Please enter your name to retrieve your secret question.</p>
            <?php if (!empty($error)): ?>
                <div class="error"><?php echo htmlspecialchars($error); ?></div>
            <?php endif; ?>
            
            <?php if (!$showSecretQuestion): ?>
                <form method="POST" action="login.php">
                    <input type="hidden" name="step" value="get_question">
                    <div class="form-group">
                        <label for="name">Name:</label>
                        <input type="text" id="name" name="name" required>
                    </div>
                    <button type="submit">Get Secret Question</button>
                </form>
            <?php else: ?>                
                <form method="POST" action="login.php">
                    <input type="hidden" name="step" value="login">
                    <input type="hidden" name="name" value="<?php echo htmlspecialchars($userName); ?>">
                    
                    <div class="form-group">
                        <label for="secretQuestion">Secret Question:</label>
                        <div class="question-display">
                            <?php echo htmlspecialchars($userDetails['secretQuestion']); ?>
                        </div>
                    </div>
                    
                    <div class="form-group">
                        <label for="secretAnswer">Your Answer:</label>
                        <input type="text" id="secretAnswer" name="secretAnswer" required>
                    </div>
                    
                    <button type="submit">Login</button>
                </form>
                
                <div class="back-link">
                    <a href="login.php">← Back to name entry</a>
                </div>
            <?php endif; ?>
        </div>
    </body>
</html> 