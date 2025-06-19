<?php
session_start();
require_once 'config/db.php';

if (empty($_SESSION['user_id'])) {
    header("Location: login.php");
    exit();
}

$totalPrice = 0;
$price = 0;
$numberOfProducts = 0;
$allCategories = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    foreach ($_SESSION['cart'] as $product) {
        $numberOfProducts = $numberOfProducts + 1;

        $price = (int) $product['price'];
        $totalPrice = $totalPrice + $price;

        $split = explode('-', $product['name']);

        array_push($allCategories, $split[0]);
    }

    if ($numberOfProducts >= 3) {
        $totalPrice = $totalPrice * 0.9;
    }

    if (count($allCategories) !== count(array_unique($allCategories))) {
        $totalPrice = $totalPrice * 0.95;
    }

    $stmt = $pdo->prepare('INSERT into Orders (userId, totalPrice) values (?, ?)');
    $stmt->execute([$_SESSION['user_id'], $totalPrice]);

    $orderId = $pdo->lastInsertId();
    foreach ($_SESSION['cart'] as $product) {
        $stmt = $pdo->prepare('INSERT into OrderItems (orderId, productId) values (?, ?)');
        $stmt->execute([$orderId, $product['id']]);
    }
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Web Exam</title>
    <link rel="stylesheet" href="css/style.css">
</head>

<div class='container'>
    <?php
    echo "<h2>Products in cart: </h2>";

    foreach ($_SESSION['cart'] as $product) {
        echo "<li>";
        echo "<h3>" . htmlspecialchars($product['name']) . "</h3>";
        echo "<p>" . htmlspecialchars($product['price']) . "</p>";
        echo "</li>";
    }
    echo "</ul>";
    ?>

    <p>Your Final Price (with Discount):</p>
    <?php
    echo "<p>" . htmlspecialchars($totalPrice) . "</p>";

    // foreach ($allCategories as $category) {
    //     echo "<p>" . htmlspecialchars($category) . "</p>";
    // }

    // echo "<hr />";

    // foreach (array_unique($allCategories) as $category) {
    //     echo "<p>" . htmlspecialchars($category) . "</p>";
    // }
    ?>

    <form method="POST" action='finalize_order.php'>
        <input type='submit' value='Finalize order' class='button'>
    </form>
</div>

</body>

</html>