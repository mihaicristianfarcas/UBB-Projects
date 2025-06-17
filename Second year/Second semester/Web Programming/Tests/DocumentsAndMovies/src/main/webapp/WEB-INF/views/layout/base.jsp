<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>${pageTitle != null ? pageTitle : 'Exam JSP'}</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body class="min-h-screen flex flex-col bg-gray-50">
    <nav class="bg-blue-600 text-white shadow-md">
        <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
            <div class="flex justify-between h-16">
                <div class="flex">
                    <div class="flex-shrink-0 flex items-center">
                        <a href="${pageContext.request.contextPath}/home" class="text-xl font-bold">Exam JSP</a>
                    </div>
                    <div class="sm:ml-6 flex space-x-8">
                        <c:if test="${not empty sessionScope.author}">
                            <a href="${pageContext.request.contextPath}/home" class="border-transparent text-white hover:bg-blue-700 hover:border-blue-500 inline-flex items-center px-1 pt-1 border-b-2 text-sm font-medium">
                                Home
                            </a>
                        </c:if>
                    </div>
                </div>
                <div class="sm:ml-6 flex items-center"></div>
                    <c:choose>
                        <c:when test="${not empty sessionScope.author}">
                            <div class="ml-3 relative">
                                <div>
                                    <button type="button" class="bg-blue-600 text-center rounded-full focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-blue-600 focus:ring-white" id="author-menu" aria-expanded="false" aria-haspopup="true">
                                        <span>${sessionScope.author.name}</span>
                                    </button>
                                </div>
                                <div class="origin-top-right absolute right-0 mt-2 w-48 rounded-md shadow-lg py-1 bg-white ring-1 ring-black ring-opacity-5 focus:outline-none hidden" role="menu" aria-orientation="vertical" aria-labelledby="author-menu">
                                    <a href="${pageContext.request.contextPath}/logout" class="block px-4 py-2 text-sm text-gray-700 hover:bg-gray-100" role="menuitem">Logout</a>
                                </div>
                            </div>
                        </c:when>
                        <c:otherwise>
                            <a href="${pageContext.request.contextPath}/login" class="border-transparent text-white hover:bg-blue-700 hover:border-blue-500 inline-flex items-center px-3 py-2 border-b-2 text-sm font-medium">
                                Login
                            </a>
                        </c:otherwise>
                    </c:choose>
                </div>
            </div>
        </div>
    </nav>

    <main class="max-w-7xl mx-auto py-6 px-4 sm:px-6 lg:px-8">
        <!-- Messages -->
        <c:if test="${not empty requestScope.message or not empty sessionScope.message}">
            <div class="mb-4 bg-green-50 border-l-4 border-green-400 p-4">
                <div class="flex">
                    <div class="flex-shrink-0">
                        <i class="fas fa-check-circle text-green-400"></i>
                    </div>
                    <div class="ml-3">
                        <p class="text-sm text-green-700">${not empty requestScope.message ? requestScope.message : sessionScope.message}</p>
                    </div>
                </div>
            </div>
            <c:remove var="message" scope="session" />
        </c:if>
        <c:if test="${not empty requestScope.error or not empty sessionScope.error}">
            <div class="mb-4 bg-red-50 border-l-4 border-red-400 p-4">
                <div class="flex">
                    <div class="flex-shrink-0">
                        <i class="fas fa-exclamation-circle text-red-400"></i>
                    </div>
                    <div class="ml-3">
                        <p class="text-sm text-red-700">${not empty requestScope.error ? requestScope.error : sessionScope.error}</p>
                    </div>
                </div>
            </div>
            <c:remove var="error" scope="session" />
        </c:if>
        
        <!-- Page Content -->
        <jsp:include page="${contentPage}" />
    </main>

    <script>
        // Author menu toggle
        document.addEventListener('DOMContentLoaded', function() {
            const authorMenuButton = document.getElementById('author-menu');
            const authorMenu = document.querySelector('[role="menu"]');

            if (authorMenuButton && authorMenu) {
                authorMenuButton.addEventListener('click', function() {
                    authorMenu.classList.toggle('hidden');
                });

                // Close the dropdown when clicking outside
                document.addEventListener('click', function(event) {
                    if (!authorMenu.contains(event.target) && !authorMenuButton.contains(event.target)) {
                        authorMenu.classList.add('hidden');
                    }
                });
            }
        });
    </script>
</body>
</html>
