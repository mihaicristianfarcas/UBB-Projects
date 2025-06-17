<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <title>Add to Cart</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .container { max-width: 600px; margin: 0 auto; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; }
        select, input[type="number"] { width: 100%; padding: 8px; border: 1px solid #ddd; }
        button { padding: 10px 20px; background: #007cba; color: white; border: none; cursor: pointer; margin-right: 10px; }
        .success { color: green; margin-top: 10px; }
        .error { color: red; margin-top: 10px; }
        .nav { margin-bottom: 20px; }
        .nav a { margin-right: 15px; color: #007cba; text-decoration: none; }
        .cart-info { background: #e7f3ff; padding: 15px; border-radius: 5px; margin-bottom: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <div class="nav">
            <a href="${pageContext.request.contextPath}/main">← Back to Main Menu</a>
        </div>
        
        <h1>Add Product to Cart</h1>
        
        <div class="cart-info">
            <strong>Current User:</strong> ${sessionScope.username}<br>
            <strong>Items in Cart:</strong> ${sessionScope.cart != null ? sessionScope.cart.size() : 0}
        </div>
        
        <c:if test="${not empty products}">
            <form action="${pageContext.request.contextPath}/order" method="post">
                <input type="hidden" name="action" value="add">
                
                <div class="form-group">
                    <label for="productId">Select Product:</label>
                    <select id="productId" name="productId" required>
                        <option value="">-- Choose a product --</option>
                        <c:forEach var="product" items="${products}">
                            <option value="${product.id}">${product.name} - ${product.description}</option>
                        </c:forEach>
                    </select>
                </div>
                
                <div class="form-group">
                    <label for="quantity">Quantity:</label>
                    <input type="number" id="quantity" name="quantity" min="1" value="1" required>
                </div>
                
                <button type="submit">Add to Cart</button>
            </form>
        </c:if>
        
        <c:if test="${empty products and empty error}">
            <p>No products available. Please <a href="${pageContext.request.contextPath}/products?action=add">add some products</a> first.</p>
        </c:if>
        
        <c:if test="${not empty message}">
            <div class="success">${message}</div>
        </c:if>
        
        <c:if test="${not empty error}">
            <div class="error">${error}</div>
        </c:if>
        
        <hr style="margin-top: 30px;">
        <p><a href="${pageContext.request.contextPath}/order?action=finalize">Go to Finalize/Cancel Order →</a></p>
    </div>
</body>
</html>
