package com.picturevoting.servlet;

import com.picturevoting.dao.PictureDAO;
import com.picturevoting.model.User;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.File;
import java.io.IOException;

@WebServlet("/delete-picture")
public class DeletePictureServlet extends HttpServlet {
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
            
            // Get the picture to verify ownership and get file path
            com.picturevoting.model.Picture picture = pictureDAO.getPictureById(pictureId);
            
            if (picture == null) {
                throw new Exception("Picture not found");
            }
            
            // Verify that the current user is the owner of the picture
            if (picture.getUserId() != user.getId()) {
                throw new Exception("You are not authorized to delete this picture");
            }
            
            // Delete the file from the filesystem
            String filePath = getServletContext().getRealPath("") + picture.getFilePath();
            File file = new File(filePath);
            if (file.exists()) {
                if (!file.delete()) {
                    throw new Exception("Failed to delete the picture file");
                }
            }
            
            // Delete the picture record from the database
            if (pictureDAO.deletePicture(pictureId, user.getId())) {
                message = "Picture deleted successfully!";
            } else {
                throw new Exception("Failed to delete picture record from database");
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
        
        response.sendRedirect(request.getContextPath() + "/home");
    }
}
