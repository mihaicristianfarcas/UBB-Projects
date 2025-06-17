package com.documentsAndMovies.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.documentsAndMovies.dao.DocumentDAO;
import com.documentsAndMovies.model.Author;
import com.documentsAndMovies.model.Document;

@WebServlet("/add_document")
public class AddDocumentServlet extends HttpServlet {
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("author") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }

        request.getRequestDispatcher("/WEB-INF/views/addDocument.jsp").forward(request, response);
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String name = request.getParameter("name");
        String contents = request.getParameter("contents");

        Document document = new Document(name, contents);
        Author author = (Author) request.getSession().getAttribute("author");
        DocumentDAO documentDAO = new DocumentDAO();
        boolean isAdded = documentDAO.insertDocument(document, author);

        if (isAdded) {
            request.setAttribute("message", "Document added successfully.");
        } else {
            request.setAttribute("error", "Failed to add document.");
        }
        request.getRequestDispatcher("/WEB-INF/views/home.jsp").forward(request, response);

    }
}
