<?php

session_start();
require_once 'config/db.php';

if (empty($_SESSION['user_id'])) {
    // User is not logged in
    header("Location: login.php");
    exit();
}

$error = '';
$propertyAddress = '';
$propertyDescription = '';
$existingPropertyId = '';

if($_SERVER['REQUEST_METHOD'] === 'POST') {
    $propertyAddress = trim(filter_input(INPUT_POST, 'property_address'));
    $propertyDescription = trim(filter_input(INPUT_POST, 'property_description'));
    $existingPropertyId = trim(filter_input(INPUT_POST, 'existing_property_id'));

    if (!empty($existingPropertyId)) {
        // Attach existing property to user
        try {
            $stmt = $pdo->prepare("SELECT id FROM property WHERE id = ?");
            $stmt->execute([$existingPropertyId]);
            $property = $stmt->fetch();
            if ($property) {
                $stmt = $pdo->prepare("INSERT INTO UserToProperties (idUser, idProperty) VALUES (?, ?)");
                $stmt->execute([$_SESSION['user_id'], $existingPropertyId]);
                header("Location: index.php");
                exit();
            } else {
                $error = "Property ID does not exist.";
            }
        } catch (PDOException $e) {
            $error = "Error linking property: " . $e->getMessage();
        }
    } elseif (!empty($propertyAddress) && !empty($propertyDescription)) {
        // Add new property and link to user
        try {
            $stmt = $pdo->prepare("INSERT INTO property (address, description) VALUES (?, ?)");
            $stmt->execute([$propertyAddress, $propertyDescription]);
            $propertyId = $pdo->lastInsertId();
            $stmt = $pdo->prepare("INSERT INTO UserToProperties (idUser, idProperty) VALUES (?, ?)");
            $stmt->execute([$_SESSION['user_id'], $propertyId]);
            header("Location: index.php");
            exit();
        } catch (PDOException $e) {
            $error = "Error adding property: " . $e->getMessage();
        }
    } else {
        $error = "All fields are required (unless using Existing Property ID).";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Add Property</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <h1>Add a Property</h1>
        
        <?php if ($error): ?>
            <div class="error"><?php echo htmlspecialchars($error); ?></div>
        <?php endif; ?>

        <form action="add_property.php" method="post">
            <label for="property_address">Property Address:</label>
            <input type="text" id="property_address" name="property_address">

            <label for="property_description">Property Description:</label>
            <input type="text" id="property_description" name="property_description">

            <label for="existing_property_id">Or link to Existing Property ID:</label>
            <input type="number" id="existing_property_id" name="existing_property_id">

            <button type="submit">Add/Link Property</button>
        </form>
    </div>
</body>
</html>