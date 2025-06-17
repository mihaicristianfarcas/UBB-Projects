<?php
require_once __DIR__ . '/../config/db.php';

try {
    // Read and execute the schema file
    $sql = file_get_contents(__DIR__ . '/schema.sql');
    $pdo->exec($sql);
    echo "Database and tables created successfully!";
} catch(PDOException $e) {
    die("Error setting up database: " . $e->getMessage());
}
?> 