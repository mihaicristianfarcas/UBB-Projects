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
    <title>PHPTest</title>
    <link rel="stylesheet" href="css/style.css">
</head>

<body>
    <?php if (isset($_GET['error'])): ?>
        <div class="error">
            <?php echo htmlspecialchars($_GET['error']); ?>
        </div>
    <?php endif; ?>
    <div class="container">
        <h1>Welcome to PHPTest</h1>
        <p>Hello, <?php echo htmlspecialchars($_SESSION['name']); ?>!</p>
    </div>

    <div class="container">
        <?php
        $properties = [];
        $stmt = $pdo->prepare("SELECT * FROM Property WHERE id IN (SELECT idProperty from UserToProperties WHERE UserToProperties.idUser = ?)");
        $stmt->execute([$_SESSION['user_id']]);
        $properties = $stmt->fetchAll(PDO::FETCH_ASSOC);

        if (empty($properties)) {
            echo "<p>No properties found. Please add a property.</p>";
        } else {
            echo "<h2>Your Properties</h2>";
        }

        echo "<ul>";
        foreach ($properties as $property) {
            echo "<li class='property'>";
            echo "<h3>" . htmlspecialchars($property['address']) . "</h3>";
            echo "<p>" . htmlspecialchars($property['description']) . "</p>";
            echo "<form method='POST' action='api/delete_property.php' onsubmit='return confirm(\"Are you sure you want to delete this property?\");'>";
            echo "<input type='hidden' name='id' value='" . htmlspecialchars($property['id']) . "'>";
            echo "<input type='submit' value='Delete' class='button'>";
            echo "</form>";
            echo "</li>";
        }
        echo "</ul>";
        ?>
        <div class="container">
            <h2>Add a property</h2>
            <a href="add_property.php" class="button">Add Property</a>
        </div>
</body>

</html>