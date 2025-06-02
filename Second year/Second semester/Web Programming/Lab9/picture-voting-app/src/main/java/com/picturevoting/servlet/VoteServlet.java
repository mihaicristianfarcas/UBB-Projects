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
    private final PictureDAO pictureDAO = new PictureDAO();
    private static final int MIN_VOTE = 1;
    private static final int MAX_VOTE = 10;

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
            int voteValue = Integer.parseInt(request.getParameter("voteValue"));
            
            // Validate vote value
            if (voteValue < MIN_VOTE || voteValue > MAX_VOTE) {
                throw new IllegalArgumentException("Vote must be between " + MIN_VOTE + " and " + MAX_VOTE);
            }
            
            // Add the vote
            if (pictureDAO.addVote(pictureId, user.getId(), voteValue)) {
                message = String.format("Vote of %d recorded successfully!", voteValue);
            } else {
                throw new Exception("Failed to record vote");
            }
        } catch (NumberFormatException e) {
            message = "Invalid input. Please enter valid numbers.";
            isError = true;
        } catch (IllegalArgumentException e) {
            message = e.getMessage();
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
