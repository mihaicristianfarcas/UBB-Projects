package com.documentsAndMovies.servlet;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.documentsAndMovies.dao.AuthorDAO;
import com.documentsAndMovies.model.Author;

import java.io.IOException;

@WebServlet("/login")
public class LoginServlet extends HttpServlet {
    private final AuthorDAO AuthorDAO = new AuthorDAO();

    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        // Check if author is already logged in
        HttpSession session = request.getSession(false);
        if (session != null && session.getAttribute("author") != null) {
            response.sendRedirect(request.getContextPath() + "/home");
            return;
        }
        // Move logout message from session to request, if present
        if (session != null && session.getAttribute("message") != null) {
            request.setAttribute("message", session.getAttribute("message"));
            session.removeAttribute("message");
        }
        // Forward to login page
        request.getRequestDispatcher("/WEB-INF/views/login.jsp").forward(request, response);
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String authorName = request.getParameter("name");
        String documentOrMovieNameOrId = request.getParameter("document_or_movie_name_or_id");

        // Basic validation
        if (authorName == null || authorName.trim().isEmpty() || documentOrMovieNameOrId == null
                || documentOrMovieNameOrId.trim().isEmpty()) {
            request.setAttribute("error", "Author name and document/movie name/id are required");
            request.getRequestDispatcher("/WEB-INF/views/login.jsp").forward(request, response);
            return;
        }

        // Authenticate author
        Author author = AuthorDAO.validateAuthor(authorName, documentOrMovieNameOrId);

        if (author != null) {
            // Create session
            HttpSession session = request.getSession();
            session.setAttribute("author", author);
            session.setMaxInactiveInterval(30 * 60); // 30 minutes

            // Redirect to home page
            response.sendRedirect(request.getContextPath() + "/home");
        } else {
            // Authentication failed
            request.setAttribute("error", "Invalid author name or document/movie name/id");
            request.getRequestDispatcher("/WEB-INF/views/login.jsp").forward(request, response);
        }
    }
}
