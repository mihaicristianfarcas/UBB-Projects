<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <title>Shopping App - Main Menu</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .container { max-width: 600px; margin: 0 auto; }
        .menu { list-style: none; padding: 0; }
        .menu li { margin-bottom: 10px; }
        .menu a { display: block; padding: 15px; background: #f0f0f0; text-decoration: none; color: #333; border-radius: 5px; }
        .menu a:hover { background: #e0e0e0; }
        .user-info { background: #e7f3ff; padding: 10px; border-radius: 5px; margin-bottom: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <div class="user-info">
            <strong>Welcome, ${sessionScope.username}!</strong>
        </div>
        
        <h1>Shopping App - Main Menu</h1>
        
        <ul class="menu">
            <li><a href="${pageContext.request.contextPath}/products?action=add">Add New Product</a></li>
            <li><a href="${pageContext.request.contextPath}/products">Search/View Products</a></li>
            <li><a href="${pageContext.request.contextPath}/order">Add Product to Cart</a></li>
            <li><a href="${pageContext.request.contextPath}/order?action=finalize">Finalize/Cancel Order</a></li>
        </ul>
        
        <hr style="margin-top: 30px;">
        <a href="${pageContext.request.contextPath}/">Change Username</a>
    </div>
</body>
</html>
