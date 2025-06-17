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
    <link rel="stylesheet" href="${pageContext.request.contextPath}/resources/css/style.css">
</head>
<body class="min-h-screen flex flex-col bg-gray-50">
    <nav class="bg-blue-600 text-white shadow-md">
        <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
            <div class="flex justify-between h-16">
                <div class="flex">
                    <div class="flex-shrink-0 flex items-center">
                        <a href="${pageContext.request.contextPath}/home" class="text-xl font-bold">Exam JSP</a>
                    </div>
                    <div class="hidden sm:ml-6 sm:flex sm:space-x-8">
                        <c:if test="${not empty sessionScope.user}">
                            <a href="${pageContext.request.contextPath}/home" class="border-transparent text-white hover:bg-blue-700 hover:border-blue-500 inline-flex items-center px-1 pt-1 border-b-2 text-sm font-medium">
                                Home
                            </a>
                        </c:if>
                    </div>
                </div>
                <div class="hidden sm:ml-6 sm:flex sm:items-center">
                    <c:choose>
                        <c:when test="${not empty sessionScope.user}">
                            <div class="ml-3 relative">
                                <div>
                                    <button type="button" class="bg-blue-600 flex text-sm rounded-full focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-blue-600 focus:ring-white" id="user-menu" aria-expanded="false" aria-haspopup="true">
                                        <span class="sr-only">Open user menu</span>
                                        <i class="fas fa-user-circle text-2xl"></i>
                                        <span class="ml-2">${sessionScope.user.username}</span>
                                    </button>
                                </div>
                                <div class="origin-top-right absolute right-0 mt-2 w-48 rounded-md shadow-lg py-1 bg-white ring-1 ring-black ring-opacity-5 focus:outline-none hidden" role="menu" aria-orientation="vertical" aria-labelledby="user-menu">
                                    <a href="${pageContext.request.contextPath}/logout" class="block px-4 py-2 text-sm text-gray-700 hover:bg-gray-100" role="menuitem">Logout</a>
                                </div>
                            </div>
                        </c:when>
                        <c:otherwise>
                            <div class="flex space-x-4">
                                <a href="${pageContext.request.contextPath}/login" class="border-transparent text-white hover:bg-blue-700 hover:border-blue-500 inline-flex items-center px-3 py-2 border-b-2 text-sm font-medium">
                                    Login
                                </a>
                                <a href="${pageContext.request.contextPath}/register" class="border-transparent text-white hover:bg-blue-700 hover:border-blue-500 inline-flex items-center px-3 py-2 border-b-2 text-sm font-medium">
                                    Register
                                </a>
                            </div>
                        </c:otherwise>
                    </c:choose>
                </div>
                <!-- Mobile menu button -->
                <div class="-mr-2 flex items-center sm:hidden">
                    <button type="button" class="inline-flex items-center justify-center p-2 rounded-md text-white hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-inset focus:ring-white" aria-controls="mobile-menu" aria-expanded="false">
                        <span class="sr-only">Open main menu</span>
                        <i class="fas fa-bars"></i>
                    </button>
                </div>
            </div>
        </div>
        <!-- Mobile menu, show/hide based on menu state. -->
        <div class="sm:hidden hidden" id="mobile-menu">
            <div class="pt-2 pb-3 space-y-1">
                <c:if test="${not empty sessionScope.user}">
                    <a href="${pageContext.request.contextPath}/home" class="bg-blue-700 border-blue-500 text-white block pl-3 pr-4 py-2 border-l-4 text-base font-medium">Home</a>
                </c:if>
                <c:choose>
                    <c:when test="${not empty sessionScope.user}">
                        <a href="${pageContext.request.contextPath}/logout" class="block pl-3 pr-4 py-2 border-l-4 border-transparent text-base font-medium text-white hover:bg-blue-700">Logout</a>
                    </c:when>
                    <c:otherwise>
                        <a href="${pageContext.request.contextPath}/login" class="block pl-3 pr-4 py-2 border-l-4 border-transparent text-base font-medium text-white hover:bg-blue-700">Login</a>
                        <a href="${pageContext.request.contextPath}/register" class="block pl-3 pr-4 py-2 border-l-4 border-transparent text-base font-medium text-white hover:bg-blue-700">Register</a>
                    </c:otherwise>
                </c:choose>
            </div>
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
        // Mobile menu toggle
        document.addEventListener('DOMContentLoaded', function() {
            const mobileMenuButton = document.querySelector('[aria-controls="mobile-menu"]');
            const mobileMenu = document.getElementById('mobile-menu');
            const userMenuButton = document.getElementById('user-menu');
            const userMenu = document.querySelector('[role="menu"]');

            if (mobileMenuButton && mobileMenu) {
                mobileMenuButton.addEventListener('click', function() {
                    mobileMenu.classList.toggle('hidden');
                });
            }

            if (userMenuButton && userMenu) {
                userMenuButton.addEventListener('click', function() {
                    userMenu.classList.toggle('hidden');
                });

                // Close the dropdown when clicking outside
                document.addEventListener('click', function(event) {
                    if (!userMenu.contains(event.target) && !userMenuButton.contains(event.target)) {
                        userMenu.classList.add('hidden');
                    }
                });
            }
        });
    </script>
    <script src="${pageContext.request.contextPath}/resources/js/main.js"></script>
</body>
</html>