package com.ordersProducts.servlet;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.ordersProducts.dao.ProductDao;
import com.ordersProducts.model.Product;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;

@WebServlet("/products")
public class ProductServlet extends HttpServlet {
    private ProductDao productDao = new ProductDao();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String action = request.getParameter("action");

        if ("search".equals(action)) {
            handleSearch(request, response);
        } else if ("add".equals(action)) {
            request.getRequestDispatcher("/WEB-INF/views/addProduct.jsp").forward(request, response);
        } else {
            // Default action: show all products
            handleShowAllProducts(request, response);
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String action = request.getParameter("action");

        if ("add".equals(action)) {
            handleAddProduct(request, response);
        } else if ("search".equals(action)) {
            handleSearch(request, response);
        }
    }

    private void handleAddProduct(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String name = request.getParameter("name");
        String description = request.getParameter("description");

        if (name != null && !name.trim().isEmpty()) {
            Product product = new Product();
            product.setName(name.trim());
            product.setDescription(description != null ? description.trim() : "");

            try {
                productDao.addProduct(product);
                request.setAttribute("message", "Product added successfully!");
            } catch (SQLException e) {
                request.setAttribute("error", "Failed to add product: " + e.getMessage());
            }
        } else {
            request.setAttribute("error", "Product name is required!");
        }

        request.getRequestDispatcher("/WEB-INF/views/addProduct.jsp").forward(request, response);
    }

    private void handleShowAllProducts(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        try {
            List<Product> products = productDao.getAllProducts();
            request.setAttribute("products", products);
        } catch (SQLException e) {
            request.setAttribute("error", "Failed to retrieve products: " + e.getMessage());
        }

        request.getRequestDispatcher("/WEB-INF/views/products.jsp").forward(request, response);
    }

    private void handleSearch(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String searchPrefix = request.getParameter("searchPrefix");

        try {
            List<Product> products;
            if (searchPrefix != null && !searchPrefix.trim().isEmpty()) {
                products = productDao.getProductsByNamePrefix(searchPrefix.trim());
                request.setAttribute("searchPrefix", searchPrefix.trim());
            } else {
                products = productDao.getAllProducts();
            }

            request.setAttribute("products", products);
        } catch (SQLException e) {
            request.setAttribute("error", "Failed to retrieve products: " + e.getMessage());
        }

        request.getRequestDispatcher("/WEB-INF/views/products.jsp").forward(request, response);
    }
}
