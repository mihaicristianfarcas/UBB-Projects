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

@WebServlet("/home")
public class HomeServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;
    private final PictureDAO pictureDAO = new PictureDAO();
    private static final int DEFAULT_TOP_COUNT = 10;

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("user") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }
        
        User user = (User) session.getAttribute("user");
        int topN = getIntParameter(request, "top", DEFAULT_TOP_COUNT);
        
        // Get top pictures and user's pictures
        request.setAttribute("topPictures", pictureDAO.getTopPictures(topN));
        request.setAttribute("userPictures", pictureDAO.getPicturesByUser(user.getId()));
        request.setAttribute("topCount", topN);
        
        request.getRequestDispatcher("/WEB-INF/views/home.jsp").forward(request, response);
    }
    
    private int getIntParameter(HttpServletRequest request, String paramName, int defaultValue) {
        try {
            return Integer.parseInt(request.getParameter(paramName));
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }
}
