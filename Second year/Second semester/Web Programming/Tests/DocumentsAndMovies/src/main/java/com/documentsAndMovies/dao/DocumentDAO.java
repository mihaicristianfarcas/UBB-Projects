package com.documentsAndMovies.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.documentsAndMovies.model.Author;
import com.documentsAndMovies.model.Document;
import com.documentsAndMovies.util.DatabaseUtil;

public class DocumentDAO {
    private static final String GET_DOCUMENT_BY_ID = "SELECT * FROM documents WHERE id = ?";
    private static final String GET_DOCUMENT_BY_NAME = "SELECT * FROM documents WHERE name = ?";
    private static final String GET_ALL_DOCUMENTS = "SELECT * FROM documents";
    private static final String INSERT_DOCUMENT = "INSERT INTO documents (name, contents) VALUES (?, ?)";

    public Document getDocumentById(int id) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_DOCUMENT_BY_ID)) {

            stmt.setInt(1, id);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                return extractDocumentFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public Document getDocumentByName(String name) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_DOCUMENT_BY_NAME)) {

            stmt.setString(1, name);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                return extractDocumentFromResultSet(rs);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public List<Document> getAllDocuments() {
        List<Document> documents = new ArrayList<>();

        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(GET_ALL_DOCUMENTS);
                ResultSet rs = stmt.executeQuery()) {

            while (rs.next()) {
                documents.add(extractDocumentFromResultSet(rs));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return documents;
    }

    public boolean insertDocument(Document document, Author author) {
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(INSERT_DOCUMENT)) {

            // Insert the new document in the current author's documentList
            AuthorDAO authorDAO = new AuthorDAO();

            String documentList = author.getDocumentList();
            String[] documents = documentList != null ? documentList.split(",") : new String[0];

            // Check if the document already exists in the author's list
            boolean documentExists = false;
            for (String doc : documents) {
                if (doc.equals(document.getName())) {
                    documentExists = true;
                    break;
                }
            }

            // If the document does not exist, add it to the list
            if (!documentExists) {
                if (documentList != null && !documentList.isEmpty()) {
                    documentList += "," + document.getName();
                } else {
                    documentList = document.getName();
                }
            }

            author.setDocumentList(documentList);
            authorDAO.updateAuthorDocumentList(author.getName(), documentList);

            stmt.setString(1, document.getName());
            stmt.setString(2, document.getContents());
            int rowsAffected = stmt.executeUpdate();
            return rowsAffected > 0;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }

    private Document extractDocumentFromResultSet(ResultSet rs) throws SQLException {
        Document doc = new Document();
        doc.setId(rs.getInt("id"));
        doc.setName(rs.getString("name"));
        doc.setContents(rs.getString("contents"));
        return doc;
    }

    public Document validateDocument(String name, String contents) {
        Document document = getDocumentByName(name);
        if (document != null && document.getName().equals(name) && document.getContents().equals(contents)) {
            return document;
        }
        return null;
    }
}
