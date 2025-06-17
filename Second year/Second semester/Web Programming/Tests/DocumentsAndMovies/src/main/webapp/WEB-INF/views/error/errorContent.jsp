<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="min-h-full flex items-center justify-center py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-2xl w-full space-y-8">
        <div class="bg-white py-8 px-4 shadow-xl rounded-lg sm:px-10">
            <div class="text-center">
                <div class="text-6xl text-red-500 mb-4">
                    <i class="fas fa-bug"></i>
                </div>
                <h1 class="text-3xl font-bold text-gray-900 mb-6">An unexpected error occurred</h1>
            </div>
            
            <c:if test="${not empty requestScope['javax.servlet.error.message']}">
                <div class="mb-4 bg-red-50 border-l-4 border-red-400 p-4">
                    <div class="flex">
                        <div class="flex-shrink-0">
                            <i class="fas fa-exclamation-circle text-red-400"></i>
                        </div>
                        <div class="ml-3">
                            <p class="text-sm text-red-700"><strong>Error:</strong> ${requestScope['javax.servlet.error.message']}</p>
                        </div>
                    </div>
                </div>
            </c:if>
            
            <c:if test="${not empty requestScope['javax.servlet.error.exception']}">
                <div class="mb-4 bg-gray-50 border-l-4 border-gray-400 p-4">
                    <div class="flex">
                        <div class="flex-shrink-0">
                            <i class="fas fa-info-circle text-gray-400"></i>
                        </div>
                        <div class="ml-3">
                            <p class="text-sm text-gray-700"><strong>Exception:</strong> ${requestScope['javax.servlet.error.exception']}</p>
                        </div>
                    </div>
                </div>
            </c:if>
            
            <c:if test="${not empty requestScope['javax.servlet.error.request_uri']}">
                <div class="mb-6 text-center">
                    <p class="text-gray-500">
                        <strong>Request URI:</strong> ${requestScope['javax.servlet.error.request_uri']}
                    </p>
                </div>
            </c:if>
            
            <div class="text-center space-x-4">
                <a href="${pageContext.request.contextPath}/home" 
                   class="inline-flex items-center px-6 py-3 border border-transparent text-base font-medium rounded-md text-white bg-blue-600 hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
                    <i class="fas fa-home mr-2"></i> Go to Home
                </a>
                <button onclick="window.history.back()" 
                        class="inline-flex items-center px-6 py-3 border border-gray-300 text-base font-medium rounded-md text-gray-700 bg-white hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
                    <i class="fas fa-arrow-left mr-2"></i> Go Back
                </button>
            </div>
            
            <!-- Only show stack trace in development environment -->
            <c:if test="${pageContext.request.serverName == 'localhost'}">
                <div class="mt-8">
                    <button class="text-sm text-gray-600 hover:text-gray-900 focus:outline-none" type="button" 
                            onclick="document.getElementById('stackTrace').classList.toggle('hidden')">
                        Show Stack Trace
                    </button>
                    <div class="hidden mt-4" id="stackTrace">
                        <div class="bg-gray-100 rounded-lg p-4">
                            <pre class="text-sm text-gray-800 whitespace-pre-wrap break-words">
<c:forEach items="${pageContext.exception.stackTrace}" var="trace">
${trace}
</c:forEach>
                            </pre>
                        </div>
                    </div>
                </div>
            </c:if>
        </div>
    </div>
</div>
