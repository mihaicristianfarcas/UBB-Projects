package com.documentsAndMovies.dao;

import java.sql.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.documentsAndMovies.model.Author;
import com.documentsAndMovies.model.Document;
import com.documentsAndMovies.model.Movie;
import com.documentsAndMovies.util.DatabaseUtil;

public class AuthorDAO {

    private static final String GET_AUTHOR_BY_ID = "SELECT * FROM authors WHERE id = ?";
    private static final String GET_AUTHOR_BY_NAME = "SELECT * FROM authors WHERE name = ?";
    private static final String GET_DOCUMENT_LIST_OF_AUTHOR = "SELECT documentList FROM authors WHERE name = ?";
    private static final String GET_MOVIE_LIST_OF_AUTHOR = "SELECT movieList FROM authors WHERE name = ?";
    private static final String GET_ALL_AUTHORS = "SELECT * FROM authors";
    private static final String SET_MOVIE_LIST = "UPDATE authors SET movieList = ? WHERE name = ?";
    private static final String SET_DOCUMENT_LIST = "UPDATE authors SET documentList = ? WHERE name = ?";

    public Author getAuthorById(int id) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_AUTHOR_BY_ID)) {

            stmt.setInt(1, id);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                return extractAuthorFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public Author getAuthorByName(String name) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_AUTHOR_BY_NAME)) {

            stmt.setString(1, name);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                return extractAuthorFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public List<Author> getAllAuthors() {
        List<Author> authors = new ArrayList<>();

        try (Connection conn = DatabaseUtil.getConnection();
                Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery(GET_ALL_AUTHORS)) {

            while (rs.next()) {
                authors.add(extractAuthorFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return authors;
    }

    public List<Document> getDocumentListOfAuthor(String authorName) {
        List<Document> documents = new ArrayList<>();
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_DOCUMENT_LIST_OF_AUTHOR)) {

            stmt.setString(1, authorName);
            ResultSet rs = stmt.executeQuery();
            DocumentDAO documentDAO = new DocumentDAO();

            if (rs.next()) {
                String documentList = rs.getString("documentList");
                if (documentList != null && !documentList.isEmpty()) {
                    String[] docNames = documentList.split(",");
                    for (String docName : docNames) {
                        docName = docName.trim();
                        if (!docName.isEmpty()) {
                            Document document = documentDAO.getDocumentByName(docName);
                            if (document != null) {
                                documents.add(document);
                            }
                        }
                    }
                }
            }
            return documents;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return documents;
    }

    public List<Movie> getMovieListOfAuthor(String authorName) {
        List<Movie> movies = new ArrayList<>();
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_MOVIE_LIST_OF_AUTHOR)) {

            stmt.setString(1, authorName);
            ResultSet rs = stmt.executeQuery();
            MovieDAO movieDAO = new MovieDAO();

            if (rs.next()) {
                String movieList = rs.getString("movieList");
                if (movieList != null && !movieList.isEmpty()) {
                    String[] movieTitles = movieList.split(",");
                    for (String title : movieTitles) {
                        title = title.trim();
                        if (!title.isEmpty()) {
                            Movie movie = movieDAO.getMovieByTitle(title);
                            if (movie != null) {
                                movies.add(movie);
                            }
                        }
                    }
                }
            }
            return movies;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return movies;
    }

    public void updateAuthorMovieList(String authorName, String movieList) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(SET_MOVIE_LIST)) {

            stmt.setString(1, movieList);
            stmt.setString(2, authorName);
            stmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void updateAuthorDocumentList(String authorName, String documentList) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(SET_DOCUMENT_LIST)) {

            stmt.setString(1, documentList);
            stmt.setString(2, authorName);
            stmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private Author extractAuthorFromResultSet(ResultSet rs) throws SQLException {
        Author author = new Author();
        author.setId(rs.getInt("id"));
        author.setName(rs.getString("name"));
        author.setDocumentList(rs.getString("documentList"));
        author.setMovieList(rs.getString("movieList"));
        return author;
    }

    public Author validateAuthor(String name, String documentOrMovieNameOrId) {
        Author author = getAuthorByName(name);
        if (author != null) {
            String documentList = author.getDocumentList();
            if (documentList != null && !documentList.trim().isEmpty()) {
                for (String doc : Arrays.asList(documentList.split(","))) {
                    if (doc.trim().equals(documentOrMovieNameOrId)) {
                        return author;
                    }
                }
            }
            String movieList = author.getMovieList();
            if (movieList != null && !movieList.trim().isEmpty()) {
                for (String movie : Arrays.asList(movieList.split(","))) {
                    if (movie.trim().equals(documentOrMovieNameOrId)) {
                        return author;
                    }
                }
            }
        }
        return null;
    }
}
