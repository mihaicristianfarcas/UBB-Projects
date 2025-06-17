<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <title>Finalize Order</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .container { max-width: 600px; margin: 0 auto; }
        .cart-summary { background: #f9f9f9; padding: 20px; border-radius: 5px; margin-bottom: 20px; }
        button { padding: 15px 30px; margin: 10px; border: none; cursor: pointer; border-radius: 5px; font-size: 16px; }
        .finalize-btn { background: #28a745; color: white; }
        .cancel-btn { background: #dc3545; color: white; }
        .success { color: green; margin-top: 10px; }
        .error { color: red; margin-top: 10px; }
        .nav { margin-bottom: 20px; }
        .nav a { margin-right: 15px; color: #007cba; text-decoration: none; }
    </style>
</head>
<body>
    <div class="container">
        <div class="nav">
            <a href="${pageContext.request.contextPath}/main">← Back to Main Menu</a>
            <a href="${pageContext.request.contextPath}/order">← Back to Add Products</a>
        </div>
        
        <h1>Finalize Order</h1>
        
        <div class="cart-summary">
            <h3>Current Cart Summary</h3>
            <strong>User:</strong> ${sessionScope.username}<br>
            <strong>Items in Cart:</strong> ${sessionScope.cart != null ? sessionScope.cart.size() : 0}
            
            <c:if test="${sessionScope.cart != null and not empty sessionScope.cart}">
                <h4>Items:</h4>
                <ul>
                    <c:forEach var="order" items="${sessionScope.cart}">
                        <li>Product ID: ${order.productId}, Quantity: ${order.quantity}</li>
                    </c:forEach>
                </ul>
            </c:if>
        </div>
        
        <c:if test="${sessionScope.cart != null and not empty sessionScope.cart}">
            <p><strong>Choose an action:</strong></p>
            
            <form action="${pageContext.request.contextPath}/order" method="post" style="display: inline;">
                <input type="hidden" name="action" value="finalize">
                <button type="submit" class="finalize-btn">Finalize Order</button>
            </form>
            
            <form action="${pageContext.request.contextPath}/order" method="post" style="display: inline;">
                <input type="hidden" name="action" value="cancel">
                <button type="submit" class="cancel-btn">Cancel Order</button>
            </form>
        </c:if>
        
        <c:if test="${sessionScope.cart == null or empty sessionScope.cart}">
            <p>Your cart is empty. <a href="${pageContext.request.contextPath}/order">Add some products to your cart</a> before finalizing an order.</p>
        </c:if>
        
        <c:if test="${not empty message}">
            <div class="success">${message}</div>
        </c:if>
        
        <c:if test="${not empty error}">
            <div class="error">${error}</div>
        </c:if>
    </div>
</body>
</html>
