-- Picture Voting Application Database Setup
-- Run this script to manually create the database and tables

-- Create database
CREATE DATABASE IF NOT EXISTS picture_voting_db;
USE picture_voting_db;

-- Create users table
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create pictures table
CREATE TABLE IF NOT EXISTS pictures (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    title VARCHAR(100) NOT NULL,
    description TEXT,
    file_path VARCHAR(255) NOT NULL,
    uploaded_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Create votes table
CREATE TABLE IF NOT EXISTS votes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    picture_id INT NOT NULL,
    voter_id INT NOT NULL,
    value INT NOT NULL DEFAULT 1,
    voted_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (picture_id) REFERENCES pictures(id) ON DELETE CASCADE,
    FOREIGN KEY (voter_id) REFERENCES users(id) ON DELETE CASCADE,
    UNIQUE KEY unique_vote (picture_id, voter_id)
);

-- Create indexes for better performance
CREATE INDEX idx_pictures_user_id ON pictures(user_id);
CREATE INDEX idx_votes_picture_id ON votes(picture_id);
CREATE INDEX idx_votes_voter_id ON votes(voter_id);

-- Insert sample data (optional)
-- Uncomment the following lines to add sample users and pictures

/*
-- Sample users (passwords are plain text for demo - in real app they should be hashed)
INSERT INTO users (username, email, password) VALUES 
('admin', 'admin@example.com', 'admin123'),
('user1', 'user1@example.com', 'password123'),
('user2', 'user2@example.com', 'password123');

-- Note: You would need to upload actual pictures and update file_path accordingly
-- INSERT INTO pictures (user_id, title, description, file_path) VALUES 
-- (1, 'Sample Picture 1', 'This is a sample picture', 'uploads/sample1.jpg'),
-- (2, 'Sample Picture 2', 'Another sample picture', 'uploads/sample2.jpg');
*/

-- Show tables to verify creation
SHOW TABLES;

-- Show table structures
DESCRIBE users;
DESCRIBE pictures;
DESCRIBE votes; 