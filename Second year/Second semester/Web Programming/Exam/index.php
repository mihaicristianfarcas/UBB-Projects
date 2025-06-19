<?php
session_start();
require_once 'config/db.php';

if (empty($_SESSION['user_id'])) {
    header("Location: login.php");
    exit();
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

<body>
    <div class="container">
        <header>
            <h1>Web Exam</h1>
            <?php if (isset($_SESSION['user_id'])): ?>
                <div class="user-actions">
                    <a href="logout.php" class="button">Logout</a>
                </div>
            <?php else: ?>
                <div class="user-actions">
                    <a href="login.php" class="button">Login</a>
                    <a href="register.php" class="button">Register</a>
                </div>
            <?php endif; ?>
        </header>
    </div>


    <div class="container">
        <?php
        $products = [];
        $stmt = $pdo->prepare("SELECT * FROM Product");
        $stmt->execute();
        $products = $stmt->fetchAll(PDO::FETCH_ASSOC);

        if (empty($products)) {
            echo "<p>No products found. Please add a product.</p>";
        } else {
            echo "<h2>Available products: </h2>";
        }
        echo "<ul>";
        foreach ($products as $product) {
            echo "<li>";
            echo "<h3>" . htmlspecialchars($product['name']) . "</h3>";
            echo "<p>" . htmlspecialchars($product['price']) . "</p>";
            echo "<form method='POST' action='api/add_product_to_cart.php'>";
            echo "<input type='hidden' name='productId' value='" . htmlspecialchars($product['id']) . "'>";
            echo "<input type='submit' value='Add to order' class='button'>";
            echo "</form>";
            echo "</li>";
        }
        echo "</ul>";

        echo "<h2>Products in cart: </h2>";

        foreach ($_SESSION['cart'] as $product) {
            echo "<li>";
            echo "<h3>" . htmlspecialchars($product['name']) . "</h3>";
            echo "<p>" . htmlspecialchars($product['price']) . "</p>";
            echo "</li>";
        }
        echo "</ul>";
        ?>

        <?php
        if (!empty($_SESSION['cart'])) {
            echo "<a href='finalize_order.php'>";
            echo "<input type='submit' value='Finalize order' class='button'>";
            echo "</a>";
        }
        ?>
    </div>

</body>

</html>