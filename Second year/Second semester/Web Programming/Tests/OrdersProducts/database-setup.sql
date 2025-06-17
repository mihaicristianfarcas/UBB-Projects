-- Database setup
CREATE DATABASE IF NOT EXISTS ordersProducts;
USE ordersProducts;

-- Create Products table
CREATE TABLE IF NOT EXISTS Products (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description TEXT
);

-- Create Orders table
CREATE TABLE IF NOT EXISTS Orders (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user VARCHAR(255) NOT NULL,
    productid INT NOT NULL,
    quantity INT NOT NULL,
    FOREIGN KEY (productid) REFERENCES Products(id)
);

-- Insert some sample products
INSERT INTO Products (name, description) VALUES 
('Laptop', 'High-performance laptop'),
('Mouse', 'Wireless mouse'),
('Keyboard', 'Mechanical keyboard'),
('Monitor', '4K display monitor');

SHOW TABLES;
