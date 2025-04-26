<?php
$host = 'www.scs.ubbcluj.ro';
$dbname = 'mihaicristianfarcas';
$username = 'mihaicristianfarcas';
$password = 'mihaicristianfarcas';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $pdo->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
} catch(PDOException $e) {
    die("Connection failed: " . $e->getMessage());
}
?> 