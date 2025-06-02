package com.picturevoting.servlet;

import com.picturevoting.dao.PictureDAO;
import com.picturevoting.model.Picture;
import com.picturevoting.model.User;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.List;
import java.util.UUID;

@WebServlet("/upload")
@MultipartConfig(fileSizeThreshold = 1024 * 1024 * 2, // 2MB
        maxFileSize = 1024 * 1024 * 10, // 10MB
        maxRequestSize = 1024 * 1024 * 50) // 50MB
public class UploadServlet extends HttpServlet {
    private final PictureDAO pictureDAO = new PictureDAO();
    private static final String UPLOAD_DIR = "uploads";

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        HttpSession session = request.getSession(false);
        if (session == null || session.getAttribute("user") == null) {
            response.sendRedirect(request.getContextPath() + "/login");
            return;
        }

        User user = (User) session.getAttribute("user");
        String message = "";
        boolean isError = false;

        // Check if request contains multipart content
        if (ServletFileUpload.isMultipartContent(request)) {
            try {
                // Create upload directory if it doesn't exist
                String uploadPath = getServletContext().getRealPath("") + File.separator + UPLOAD_DIR;
                File uploadDir = new File(uploadPath);
                if (!uploadDir.exists()) {
                    uploadDir.mkdirs();
                }

                // Configure upload settings
                DiskFileItemFactory factory = new DiskFileItemFactory();
                factory.setRepository(new File(System.getProperty("java.io.tmpdir")));
                
                ServletFileUpload upload = new ServletFileUpload(factory);
                
                // Parse the request to get file items
                List<FileItem> formItems = upload.parseRequest(request);
                
                String title = "";
                String description = "";
                String fileName = "";
                
                // Process form fields and file
                for (FileItem item : formItems) {
                    if (item.isFormField()) {
                        // Process regular form fields
                        String fieldName = item.getFieldName();
                        String fieldValue = item.getString("UTF-8");
                        
                        if ("title".equals(fieldName)) {
                            title = fieldValue;
                        } else if ("description".equals(fieldName)) {
                            description = fieldValue;
                        }
                    } else {
                        // Process uploaded file
                        if (!item.getName().isEmpty()) {
                            // Generate a unique file name
                            String originalFileName = new File(item.getName()).getName();
                            String fileExtension = "";
                            int lastDot = originalFileName.lastIndexOf('.');
                            if (lastDot > 0) {
                                fileExtension = originalFileName.substring(lastDot);
                            }
                            fileName = UUID.randomUUID().toString() + fileExtension;
                            
                            // Save the file
                            try (InputStream fileContent = item.getInputStream()) {
                                Path filePath = Paths.get(uploadPath + File.separator + fileName);
                                Files.copy(fileContent, filePath, StandardCopyOption.REPLACE_EXISTING);
                            }
                        }
                    }
                }
                
                // Validate input
                if (title.trim().isEmpty()) {
                    throw new Exception("Title is required");
                }
                
                if (fileName.isEmpty()) {
                    throw new Exception("Please select a file to upload");
                }
                
                // Create picture record in database
                Picture picture = new Picture(user.getId(), title, description, UPLOAD_DIR + "/" + fileName);
                if (pictureDAO.addPicture(picture)) {
                    message = "File uploaded successfully!";
                } else {
                    throw new Exception("Failed to save picture information to database");
                }
                
            } catch (Exception ex) {
                message = "Error uploading file: " + ex.getMessage();
                isError = true;
                ex.printStackTrace();
            }
        } else {
            message = "No file part in the request";
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
