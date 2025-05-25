package com.picturevoting.util;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class DatabaseUtil {
    private static final Logger LOGGER = Logger.getLogger(DatabaseUtil.class.getName());
    private static String DB_URL;
    private static String DB_USER;
    private static String DB_PASSWORD;
    
    // Load database configuration from properties file
    static {
        Properties props = new Properties();
        try (InputStream input = DatabaseUtil.class.getClassLoader().getResourceAsStream("database.properties")) {
            if (input == null) {
                LOGGER.log(Level.WARNING, "database.properties file not found, using default values");
                // Default values as fallback
                DB_URL = "jdbc:mysql://localhost:3306/picture_voting_db";
                DB_USER = "root";
                DB_PASSWORD = "password";
            } else {
                props.load(input);
                DB_URL = props.getProperty("db.url", "jdbc:mysql://localhost:3306/picture_voting_db");
                DB_USER = props.getProperty("db.user", "root");
                DB_PASSWORD = props.getProperty("db.password", "password");
            }
        } catch (IOException e) {
            LOGGER.log(Level.SEVERE, "Could not load database properties", e);
            // Default values as fallback
            DB_URL = "jdbc:mysql://localhost:3306/picture_voting_db";
            DB_USER = "root";
            DB_PASSWORD = "password";
        }
    }
    
    // Initialize JDBC driver and database
    static {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            initializeDatabase();
        } catch (ClassNotFoundException e) {
            LOGGER.log(Level.SEVERE, "MySQL JDBC Driver not found", e);
            throw new RuntimeException("Failed to load MySQL JDBC Driver", e);
        }
    }
    
    public static Connection getConnection() throws SQLException {
        return DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD);
    }
    
    private static void initializeDatabase() {
        try (Connection conn = getConnection()) {
            // Create users table
            conn.createStatement().executeUpdate(
                "CREATE TABLE IF NOT EXISTS users (" +
                "id INT AUTO_INCREMENT PRIMARY KEY, " +
                "username VARCHAR(50) UNIQUE NOT NULL, " +
                "password VARCHAR(100) NOT NULL, " +
                "email VARCHAR(100) UNIQUE NOT NULL, " +
                "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP" +
                ")"
            );
            
            // Create pictures table
            conn.createStatement().executeUpdate(
                "CREATE TABLE IF NOT EXISTS pictures (" +
                "id INT AUTO_INCREMENT PRIMARY KEY, " +
                "user_id INT NOT NULL, " +
                "title VARCHAR(100) NOT NULL, " +
                "description TEXT, " +
                "file_path VARCHAR(255) NOT NULL, " +
                "uploaded_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, " +
                "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE" +
                ")"
            );
            
            // Create votes table
            conn.createStatement().executeUpdate(
                "CREATE TABLE IF NOT EXISTS votes (" +
                "id INT AUTO_INCREMENT PRIMARY KEY, " +
                "picture_id INT NOT NULL, " +
                "voter_id INT NOT NULL, " +
                "value INT NOT NULL DEFAULT 1, " +
                "voted_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, " +
                "FOREIGN KEY (picture_id) REFERENCES pictures(id) ON DELETE CASCADE, " +
                "FOREIGN KEY (voter_id) REFERENCES users(id) ON DELETE CASCADE, " +
                "UNIQUE KEY unique_vote (picture_id, voter_id)" +
                ")"
            );
            
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Failed to initialize database", e);
            throw new RuntimeException("Failed to initialize database", e);
        }
    }
}
