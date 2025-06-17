package com.documentsAndMovies.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.documentsAndMovies.dao.MovieDAO;

@WebServlet("/remove_movie")
public class RemoveMovieServlet extends HttpServlet {

    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("author") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }

        request.getRequestDispatcher("/WEB-INF/views/removeMovie.jsp").forward(request, response);
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String movieName = request.getParameter("title");

        MovieDAO movieDAO = new MovieDAO();
        boolean isRemoved = movieDAO.deleteMovieByTitle(movieName);
        if (isRemoved) {
            request.setAttribute("message", "Movie removed successfully.");
        } else {
            request.setAttribute("error", "Failed to remove movie. It may not exist.");
        }

        request.getRequestDispatcher("/WEB-INF/views/home.jsp").forward(request, response);
    }
}
