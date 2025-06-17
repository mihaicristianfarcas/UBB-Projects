-- Database setup
CREATE DATABASE IF NOT EXISTS exam_jsp;
USE exam_jsp;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Show tables to verify
SHOW TABLES;

-- Show table structure
DESCRIBE users;