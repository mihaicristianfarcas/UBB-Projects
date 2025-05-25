package com.picturevoting.dao;

import com.picturevoting.model.Picture;
import com.picturevoting.util.DatabaseUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class PictureDAO {
    
    private static final String INSERT_PICTURE = "INSERT INTO pictures (user_id, title, description, file_path) VALUES (?, ?, ?, ?)";
    private static final String GET_PICTURE_BY_ID = "SELECT p.*, u.username as uploader_username FROM pictures p " +
                                                   "JOIN users u ON p.user_id = u.id WHERE p.id = ?";
    private static final String GET_ALL_PICTURES = "SELECT p.*, u.username as uploader_username, " +
                                                  "COALESCE(SUM(v.value), 0) as vote_count " +
                                                  "FROM pictures p " +
                                                  "JOIN users u ON p.user_id = u.id " +
                                                  "LEFT JOIN votes v ON p.id = v.picture_id " +
                                                  "GROUP BY p.id " +
                                                  "ORDER BY vote_count DESC";
    private static final String GET_PICTURES_BY_USER = "SELECT p.*, u.username as uploader_username, " +
                                                      "COALESCE(SUM(v.value), 0) as vote_count " +
                                                      "FROM pictures p " +
                                                      "JOIN users u ON p.user_id = u.id " +
                                                      "LEFT JOIN votes v ON p.id = v.picture_id " +
                                                      "WHERE p.user_id = ? " +
                                                      "GROUP BY p.id " +
                                                      "ORDER BY p.uploaded_at DESC";
    private static final String GET_TOP_PICTURES = "SELECT p.*, u.username as uploader_username, " +
                                                  "COALESCE(SUM(v.value), 0) as vote_count " +
                                                  "FROM pictures p " +
                                                  "JOIN users u ON p.user_id = u.id " +
                                                  "LEFT JOIN votes v ON p.id = v.picture_id " +
                                                  "GROUP BY p.id " +
                                                  "ORDER BY vote_count DESC " +
                                                  "LIMIT ?";
    private static final String ADD_VOTE = "INSERT INTO votes (picture_id, voter_id, value) VALUES (?, ?, 1) " +
                                         "ON DUPLICATE KEY UPDATE value = value + 1";
    private static final String DELETE_PICTURE = "DELETE FROM pictures WHERE id = ? AND user_id = ?";
    
    public boolean addPicture(Picture picture) {
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(INSERT_PICTURE, Statement.RETURN_GENERATED_KEYS)) {
            
            stmt.setInt(1, picture.getUserId());
            stmt.setString(2, picture.getTitle());
            stmt.setString(3, picture.getDescription());
            stmt.setString(4, picture.getFilePath());
            
            int affectedRows = stmt.executeUpdate();
            
            if (affectedRows == 0) {
                return false;
            }
            
            try (ResultSet generatedKeys = stmt.getGeneratedKeys()) {
                if (generatedKeys.next()) {
                    picture.setId(generatedKeys.getInt(1));
                } else {
                    throw new SQLException("Creating picture failed, no ID obtained.");
                }
            }
            
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    public Picture getPictureById(int id) {
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(GET_PICTURE_BY_ID)) {
            
            stmt.setInt(1, id);
            ResultSet rs = stmt.executeQuery();
            
            if (rs.next()) {
                return extractPictureFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }
    
    public List<Picture> getAllPictures() {
        List<Picture> pictures = new ArrayList<>();
        
        try (Connection conn = DatabaseUtil.getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(GET_ALL_PICTURES)) {
            
            while (rs.next()) {
                pictures.add(extractPictureFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return pictures;
    }
    
    public List<Picture> getPicturesByUser(int userId) {
        List<Picture> pictures = new ArrayList<>();
        
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(GET_PICTURES_BY_USER)) {
            
            stmt.setInt(1, userId);
            ResultSet rs = stmt.executeQuery();
            
            while (rs.next()) {
                pictures.add(extractPictureFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return pictures;
    }
    
    public List<Picture> getTopPictures(int limit) {
        List<Picture> pictures = new ArrayList<>();
        
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(GET_TOP_PICTURES)) {
            
            stmt.setInt(1, limit);
            ResultSet rs = stmt.executeQuery();
            
            while (rs.next()) {
                pictures.add(extractPictureFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return pictures;
    }
    
    public boolean addVote(int pictureId, int voterId) {
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(ADD_VOTE)) {
            
            stmt.setInt(1, pictureId);
            stmt.setInt(2, voterId);
            
            int affectedRows = stmt.executeUpdate();
            return affectedRows > 0;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    public boolean deletePicture(int pictureId, int userId) {
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement stmt = conn.prepareStatement(DELETE_PICTURE)) {
            
            stmt.setInt(1, pictureId);
            stmt.setInt(2, userId);
            
            int affectedRows = stmt.executeUpdate();
            return affectedRows > 0;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    private Picture extractPictureFromResultSet(ResultSet rs) throws SQLException {
        Picture picture = new Picture();
        picture.setId(rs.getInt("id"));
        picture.setUserId(rs.getInt("user_id"));
        picture.setTitle(rs.getString("title"));
        picture.setDescription(rs.getString("description"));
        picture.setFilePath(rs.getString("file_path"));
        picture.setUploadedAt(rs.getTimestamp("uploaded_at").toLocalDateTime());
        
        try {
            picture.setVoteCount(rs.getInt("vote_count"));
        } catch (SQLException e) {
            // vote_count might not be in all result sets
            picture.setVoteCount(0);
        }
        
        try {
            picture.setUploaderUsername(rs.getString("uploader_username"));
        } catch (SQLException e) {
            // uploader_username might not be in all result sets
        }
        
        return picture;
    }
}
