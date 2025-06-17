package com.exam_jsp.util;

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
                DB_URL = "jdbc:mysql://localhost:3306/exam_jsp";
                DB_USER = "root";
                DB_PASSWORD = "";
            } else {
                props.load(input);
                DB_URL = props.getProperty("db.url", "jdbc:mysql://localhost:3306/exam_jsp");
                DB_USER = props.getProperty("db.user", "root");
                DB_PASSWORD = props.getProperty("db.password", "");
            }
        } catch (IOException e) {
            LOGGER.log(Level.SEVERE, "Could not load database properties", e);
            // Default values as fallback
            DB_URL = "jdbc:mysql://localhost:3306/exam_jsp";
            DB_USER = "root";
            DB_PASSWORD = "";
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
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Failed to initialize database", e);
            throw new RuntimeException("Failed to initialize database", e);
        }
    }
}
