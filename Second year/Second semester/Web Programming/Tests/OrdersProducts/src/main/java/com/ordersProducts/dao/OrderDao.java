package com.ordersProducts.dao;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ordersProducts.model.Order;
import com.ordersProducts.util.DatabaseUtil;

public class OrderDao {

    public void addOrder(Order order) throws SQLException {
        String sql = "INSERT INTO Orders (user, productid, quantity) VALUES (?, ?, ?)";
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, order.getUser());
            stmt.setInt(2, order.getProductId());
            stmt.setInt(3, order.getQuantity());
            stmt.executeUpdate();
        }
    }

    public void addOrders(List<Order> orders) throws SQLException {
        String sql = "INSERT INTO Orders (user, productid, quantity) VALUES (?, ?, ?)";
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(sql)) {
            for (Order order : orders) {
                stmt.setString(1, order.getUser());
                stmt.setInt(2, order.getProductId());
                stmt.setInt(3, order.getQuantity());
                stmt.addBatch();
            }
            stmt.executeBatch();
        }
    }

    public List<Order> getOrdersByUser(String user) throws SQLException {
        List<Order> orders = new ArrayList<>();
        String sql = "SELECT * FROM Orders WHERE user = ?";
        try (Connection conn = DatabaseUtil.getConnection();
                PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, user);
            ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
                Order order = new Order();
                order.setId(rs.getInt("id"));
                order.setUser(rs.getString("user"));
                order.setProductId(rs.getInt("productid"));
                order.setQuantity(rs.getInt("quantity"));
                orders.add(order);
            }
        }
        return orders;
    }
}
