package com.picturevoting.servlet;

import com.picturevoting.dao.UserDAO;
import com.picturevoting.model.User;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.regex.Pattern;

@WebServlet("/register")
public class RegisterServlet extends HttpServlet {
    private final UserDAO userDAO = new UserDAO();
    private static final Pattern EMAIL_PATTERN = Pattern.compile(
        "^[A-Za-z0-9+_.-]+@(.+)$");

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // Forward to registration page
        request.getRequestDispatcher("/WEB-INF/views/register.jsp").forward(request, response);
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String username = request.getParameter("username");
        String email = request.getParameter("email");
        String password = request.getParameter("password");
        String confirmPassword = request.getParameter("confirmPassword");
        
        // Validate input
        String error = validateInput(username, email, password, confirmPassword);
        
        if (error != null) {
            request.setAttribute("error", error);
            request.setAttribute("username", username);
            request.setAttribute("email", email);
            request.getRequestDispatcher("/WEB-INF/views/register.jsp").forward(request, response);
            return;
        }
        
        // Check if username or email already exists
        if (userDAO.getUserByUsername(username) != null) {
            request.setAttribute("error", "Username already exists");
            request.setAttribute("email", email);
            request.getRequestDispatcher("/WEB-INF/views/register.jsp").forward(request, response);
            return;
        }
        
        if (userDAO.getUserByEmail(email) != null) {
            request.setAttribute("error", "Email already registered");
            request.setAttribute("username", username);
            request.getRequestDispatcher("/WEB-INF/views/register.jsp").forward(request, response);
            return;
        }
        
        // Create new user
        User user = new User(username, email, password);
        
        if (userDAO.createUser(user)) {
            // Registration successful, redirect to login page
            request.setAttribute("success", "Registration successful! Please login.");
            response.sendRedirect(request.getContextPath() + "/login");
        } else {
            // Registration failed
            request.setAttribute("error", "Registration failed. Please try again.");
            request.setAttribute("username", username);
            request.setAttribute("email", email);
            request.getRequestDispatcher("/WEB-INF/views/register.jsp").forward(request, response);
        }
    }
    
    private String validateInput(String username, String email, String password, String confirmPassword) {
        if (username == null || username.trim().length() < 3 || username.trim().length() > 50) {
            return "Username must be between 3 and 50 characters";
        }
        
        if (email == null || !EMAIL_PATTERN.matcher(email).matches()) {
            return "Please enter a valid email address";
        }
        
        if (password == null || password.length() < 6) {
            return "Password must be at least 6 characters long";
        }
        
        if (!password.equals(confirmPassword)) {
            return "Passwords do not match";
        }
        
        return null;
    }
}
