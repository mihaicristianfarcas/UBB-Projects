<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <title>Products</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .container { max-width: 800px; margin: 0 auto; }
        .search-form { background: #f9f9f9; padding: 20px; border-radius: 5px; margin-bottom: 20px; }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; }
        input[type="text"] { width: 200px; padding: 8px; border: 1px solid #ddd; }
        button { padding: 10px 20px; background: #007cba; color: white; border: none; cursor: pointer; margin-right: 10px; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; }
        th, td { padding: 10px; border: 1px solid #ddd; text-align: left; }
        th { background: #f0f0f0; }
        .error { color: red; margin-top: 10px; }
        .nav { margin-bottom: 20px; }
        .nav a { margin-right: 15px; color: #007cba; text-decoration: none; }
    </style>
</head>
<body>
    <div class="container">
        <div class="nav">
            <a href="${pageContext.request.contextPath}/main">← Back to Main Menu</a>
        </div>
        
        <h1>Products</h1>
        
        <div class="search-form">
            <form action="${pageContext.request.contextPath}/products" method="post">
                <input type="hidden" name="action" value="search">
                
                <div class="form-group">
                    <label for="searchPrefix">Search products by name prefix:</label>
                    <input type="text" id="searchPrefix" name="searchPrefix" value="${searchPrefix}" placeholder="Enter prefix...">
                    <button type="submit">Search</button>
                </div>
            </form>
            
            <form action="${pageContext.request.contextPath}/products" method="get">
                <button type="submit">Show All Products</button>
            </form>
        </div>
        
        <c:if test="${not empty error}">
            <div class="error">${error}</div>
        </c:if>
        
        <c:if test="${not empty products}">
            <h2>
                <c:choose>
                    <c:when test="${not empty searchPrefix}">
                        Products starting with "${searchPrefix}" (${products.size()} found)
                    </c:when>
                    <c:otherwise>
                        All Products (${products.size()} total)
                    </c:otherwise>
                </c:choose>
            </h2>
            
            <table>
                <thead>
                    <tr>
                        <th>ID</th>
                        <th>Name</th>
                        <th>Description</th>
                    </tr>
                </thead>
                <tbody>
                    <c:forEach var="product" items="${products}">
                        <tr>
                            <td>${product.id}</td>
                            <td>${product.name}</td>
                            <td>${product.description}</td>
                        </tr>
                    </c:forEach>
                </tbody>
            </table>
        </c:if>
        
        <c:if test="${empty products and empty error}">
            <p>No products found. Use the search form above or <a href="${pageContext.request.contextPath}/products?action=add">add a new product</a>.</p>
        </c:if>
    </div>
</body>
</html>
