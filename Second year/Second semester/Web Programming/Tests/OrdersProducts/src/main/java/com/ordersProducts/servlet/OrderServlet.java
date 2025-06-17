package com.ordersProducts.servlet;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.ordersProducts.dao.OrderDao;
import com.ordersProducts.dao.ProductDao;
import com.ordersProducts.model.Order;
import com.ordersProducts.model.Product;

import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

@WebServlet("/order")
public class OrderServlet extends HttpServlet {
    private OrderDao orderDao = new OrderDao();
    private ProductDao productDao = new ProductDao();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String action = request.getParameter("action");

        if ("finalize".equals(action)) {
            request.getRequestDispatcher("/WEB-INF/views/finalizeOrder.jsp").forward(request, response);
        } else {
            // Show order page
            try {
                List<Product> products = productDao.getAllProducts();
                request.setAttribute("products", products);
            } catch (SQLException e) {
                request.setAttribute("error", "Failed to load products: " + e.getMessage());
            }
            request.getRequestDispatcher("/WEB-INF/views/order.jsp").forward(request, response);
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String action = request.getParameter("action");
        HttpSession session = request.getSession();

        if ("add".equals(action)) {
            handleAddToCart(request, response, session);
        } else if ("finalize".equals(action)) {
            handleFinalizeOrder(request, response, session);
        } else if ("cancel".equals(action)) {
            handleCancelOrder(request, response, session);
        }
    }

    @SuppressWarnings("unchecked")
    private void handleAddToCart(HttpServletRequest request, HttpServletResponse response, HttpSession session)
            throws ServletException, IOException {
        String username = (String) session.getAttribute("username");
        if (username == null) {
            response.sendRedirect(request.getContextPath() + "/");
            return;
        }

        String productIdStr = request.getParameter("productId");
        String quantityStr = request.getParameter("quantity");

        try {
            int productId = Integer.parseInt(productIdStr);
            int quantity = Integer.parseInt(quantityStr);

            if (quantity <= 0) {
                request.setAttribute("error", "Quantity must be greater than 0!");
            } else {
                // Add to temporary cart in session
                List<Order> cart = (List<Order>) session.getAttribute("cart");
                if (cart == null) {
                    cart = new ArrayList<>();
                    session.setAttribute("cart", cart);
                }

                Order order = new Order();
                order.setUser(username);
                order.setProductId(productId);
                order.setQuantity(quantity);
                cart.add(order);

                // Get product name for display
                Product product = productDao.getProductById(productId);
                String productName = product != null ? product.getName() : "Unknown";

                request.setAttribute("message", "Added " + quantity + " x " + productName + " to cart!");
            }
        } catch (NumberFormatException e) {
            request.setAttribute("error", "Invalid product ID or quantity!");
        } catch (SQLException e) {
            request.setAttribute("error", "Database error: " + e.getMessage());
        }

        // Reload products and forward to order page
        try {
            List<Product> products = productDao.getAllProducts();
            request.setAttribute("products", products);
        } catch (SQLException e) {
            request.setAttribute("error", "Failed to load products: " + e.getMessage());
        }
        request.getRequestDispatcher("/WEB-INF/views/order.jsp").forward(request, response);
    }

    @SuppressWarnings("unchecked")
    private void handleFinalizeOrder(HttpServletRequest request, HttpServletResponse response, HttpSession session)
            throws ServletException, IOException {
        List<Order> cart = (List<Order>) session.getAttribute("cart");

        if (cart != null && !cart.isEmpty()) {
            try {
                orderDao.addOrders(cart);
                session.removeAttribute("cart");
                request.setAttribute("message", "Order finalized successfully! " + cart.size() + " items ordered.");
            } catch (SQLException e) {
                request.setAttribute("error", "Failed to finalize order: " + e.getMessage());
            }
        } else {
            request.setAttribute("message", "No items in cart to finalize.");
        }

        request.getRequestDispatcher("/WEB-INF/views/finalizeOrder.jsp").forward(request, response);
    }

    private void handleCancelOrder(HttpServletRequest request, HttpServletResponse response, HttpSession session)
            throws ServletException, IOException {
        List<?> cart = (List<?>) session.getAttribute("cart");
        int itemCount = cart != null ? cart.size() : 0;

        session.removeAttribute("cart");
        request.setAttribute("message", "Order cancelled. " + itemCount + " items removed from cart.");
        request.getRequestDispatcher("/WEB-INF/views/finalizeOrder.jsp").forward(request, response);
    }
}
