package com.documentsAndMovies.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.documentsAndMovies.model.Author;
import com.documentsAndMovies.model.Movie;
import com.documentsAndMovies.util.DatabaseUtil;

public class MovieDAO {
    private static final String GET_MOVIE_BY_ID = "SELECT * FROM movies WHERE id = ?";
    private static final String GET_MOVIE_BY_TITLE = "SELECT * FROM movies WHERE title = ?";
    private static final String GET_ALL_MOVIES = "SELECT * FROM movies";
    private static final String DELETE_MOVIE_BY_ID = "DELETE FROM MOVIES WHERE id = ?";

    public Movie getMovieById(int id) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_MOVIE_BY_ID)) {
            stmt.setInt(1, id);

            ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
                return extractMovieFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public Movie getMovieByTitle(String title) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_MOVIE_BY_TITLE)) {
            stmt.setString(1, title);

            ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
                return extractMovieFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public List<Movie> getAllMovies() {
        List<Movie> movies = new ArrayList<>();

        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_ALL_MOVIES);
                ResultSet rs = stmt.executeQuery()) {

            while (rs.next()) {
                movies.add(extractMovieFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return movies;
    }

    public boolean deleteMovieById(int id) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(DELETE_MOVIE_BY_ID)) {
            stmt.setInt(1, id);

            AuthorDAO authorDAO = new AuthorDAO();
            List<Author> authors = authorDAO.getAllAuthors();
            for (Author author : authors) {
                String movieList = author.getMovieList();
                if (movieList != null && movieList.contains(String.valueOf(id))) {
                    movieList = movieList.replace(String.valueOf(id), "").replace(",,", ",").replaceAll("^,|,$", "");
                    author.setMovieList(movieList);
                    authorDAO.updateAuthorMovieList(author.getName(), movieList);
                }
            }

            int rowsAffected = stmt.executeUpdate();
            return rowsAffected > 0;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean deleteMovieByTitle(String title) {
        Movie movie = getMovieByTitle(title);
        if (movie != null) {
            return deleteMovieById(movie.getId());
        }
        return false;
    }

    public Movie extractMovieFromResultSet(ResultSet rs) throws SQLException {
        Movie movie = new Movie();
        movie.setId(rs.getInt("id"));
        movie.setTitle(rs.getString("title"));
        movie.setDuration(rs.getInt("duration"));
        return movie;
    }
}
