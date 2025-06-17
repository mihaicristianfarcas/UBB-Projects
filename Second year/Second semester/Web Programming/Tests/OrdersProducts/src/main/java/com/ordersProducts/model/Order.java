package com.ordersProducts.model;

public class Order {
    private int id;
    private String user;
    private int productId;
    private int quantity;

    public Order() {
    }

    public Order(int id, String user, int productId, int quantity) {
        this.id = id;
        this.user = user;
        this.productId = productId;
        this.quantity = quantity;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUser() {
        return user;
    }

    public void setUser(String user) {
        this.user = user;
    }

    public int getProductId() {
        return productId;
    }

    public void setProductId(int productId) {
        this.productId = productId;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }
}
