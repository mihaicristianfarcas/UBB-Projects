<?php
session_start();
require_once '../config/db.php';

if (empty($_SESSION['user_id'])) {
    header("Location: login.php");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $productId = filter_input(INPUT_POST, 'productId');
    $stmt = $pdo->prepare('SELECT * from product where id = ?');
    $stmt->execute([$productId]);
    $product = $stmt->fetch();

    array_push($_SESSION['cart'], $product);
    header("Location: ../index.php");
}
