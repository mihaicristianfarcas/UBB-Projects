<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="bg-white overflow-hidden shadow rounded-lg">
    <div class="px-4 py-5 sm:p-6">
        <h1 class="text-3xl font-bold text-gray-900">Welcome to Exam JSP</h1>
        <p class="mt-2 text-lg text-gray-600">You are successfully logged in!</p>
    </div>

    <div class="flex-col">    
        <button class="mt-4 mb-4 ml-4 px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-blue-500">
            <a href="${pageContext.request.contextPath}/add_document">Add Document</a>
        </button>
    
        <c:if test="${not empty documents}">
            <h2 class="text-2xl font-semibold text-gray-800 ml-4">Your Documents</h2>
            <ul class="list-disc ml-8 mt-2">
                <c:forEach items="${documents}" var="document">
                    <li class="text-gray-700">
                            ${document.toString()}
                        </a>
                    </li>
                </c:forEach>
            </ul>
        </c:if>
        <c:if test="${empty documents}">
            <p class="text-gray-600 ml-4">You have no documents yet. Please add some.</p>
        </c:if>

        <c:if test="${not empty movies}">
            <h2 class="text-2xl font-semibold text-gray-800 ml-4">Your Movies</h2>
            <ul class="list-disc ml-8 mt-2">
                <c:forEach items="${movies}" var="movie">
                    <li class="text-gray-700">
                        ${movie.toString()}
                        <form action="${pageContext.request.contextPath}/remove_movie" method="post" class="inline">
                            <button type="submit" name="title" value="${movie.title}"
                                class="ml-2 px-2 py-1 bg-red-600 text-white rounded hover:bg-red-700 focus:outline-none focus:ring-2 focus:ring-red-500">
                                Remove
                            </button>
                        </form>
                    </li>
                </c:forEach>
            </ul>
        </c:if>
        <c:if test="${empty movies}">
            <p class="text-gray-600 ml-4">You have no movies yet. Please add some.</p>
        </c:if>
    </div>
</div>
