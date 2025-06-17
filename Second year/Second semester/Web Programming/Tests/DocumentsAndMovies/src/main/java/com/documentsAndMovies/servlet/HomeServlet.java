package com.documentsAndMovies.servlet;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.documentsAndMovies.dao.AuthorDAO;
import com.documentsAndMovies.dao.DocumentDAO;
import com.documentsAndMovies.dao.MovieDAO;
import com.documentsAndMovies.model.Author;
import com.documentsAndMovies.model.Document;
import com.documentsAndMovies.model.Movie;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

@WebServlet("/home")
public class HomeServlet extends HttpServlet {
    private final AuthorDAO authorDAO = new AuthorDAO();

    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("author") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }

        Author author = (Author) session.getAttribute("author");
        List<Document> documentList = authorDAO.getDocumentListOfAuthor(author.getName());
        List<Movie> movieList = authorDAO.getMovieListOfAuthor(author.getName());

        request.setAttribute("documents", documentList);
        request.setAttribute("movies", movieList);

        // Forward to the home page
        request.getRequestDispatcher("/WEB-INF/views/home.jsp").forward(request, response);
    }
}
