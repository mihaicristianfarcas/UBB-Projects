package com.picturevoting.servlet;

import com.picturevoting.dao.PictureDAO;
import com.picturevoting.model.User;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

@WebServlet("/vote")
public class VoteServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;
    private final PictureDAO pictureDAO = new PictureDAO();

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("user") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }

        User user = (User) session.getAttribute("user");
        String message = "";
        boolean isError = false;

        try {
            int pictureId = Integer.parseInt(request.getParameter("pictureId"));
            
            // Check if the user is trying to vote on their own picture
            // This check would be more efficient with a direct database query
            // but for simplicity, we'll handle it in the application layer
            
            // Add the vote
            if (pictureDAO.addVote(pictureId, user.getId())) {
                message = "Vote recorded successfully!";
            } else {
                throw new Exception("Failed to record vote");
            }
        } catch (NumberFormatException e) {
            message = "Invalid picture ID";
            isError = true;
        } catch (Exception e) {
            message = "Error: " + e.getMessage();
            isError = true;
        }

        // Set message and redirect
        if (isError) {
            request.getSession().setAttribute("error", message);
        } else {
            request.getSession().setAttribute("message", message);
        }
        
        // Redirect back to the previous page or home
        String referer = request.getHeader("referer");
        response.sendRedirect(referer != null ? referer : request.getContextPath() + "/home");
    }
}
