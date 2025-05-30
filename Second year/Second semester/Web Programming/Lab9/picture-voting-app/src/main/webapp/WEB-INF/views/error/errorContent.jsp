<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="container text-center py-5">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card shadow">
                <div class="card-body p-5">
                    <div class="display-1 text-danger mb-4">
                        <i class="bi bi-bug"></i>
                    </div>
                    <h1 class="h2 mb-3">An unexpected error occurred</h1>
                    
                    <c:if test="${not empty requestScope['javax.servlet.error.message']}">
                        <div class="alert alert-danger text-start">
                            <strong>Error:</strong> ${requestScope['javax.servlet.error.message']}
                        </div>
                    </c:if>
                    
                    <c:if test="${not empty requestScope['javax.servlet.error.exception']}">
                        <div class="alert alert-secondary text-start">
                            <strong>Exception:</strong> ${requestScope['javax.servlet.error.exception']}
                        </div>
                    </c:if>
                    
                    <c:if test="${not empty requestScope['javax.servlet.error.request_uri']}">
                        <p class="text-muted">
                            <strong>Request URI:</strong> ${requestScope['javax.servlet.error.request_uri']}
                        </p>
                    </c:if>
                    
                    <div class="mt-5">
                        <a href="${pageContext.request.contextPath}/home" class="btn btn-primary me-3">
                            <i class="bi bi-house-door"></i> Go to Home
                        </a>
                        <button onclick="window.history.back()" class="btn btn-outline-secondary">
                            <i class="bi bi-arrow-left"></i> Go Back
                        </button>
                    </div>
                    
                    <!-- Only show stack trace in development environment -->
                    <c:if test="${pageContext.request.serverName == 'localhost'}">
                        <div class="mt-4 text-start">
                            <button class="btn btn-sm btn-outline-secondary" type="button" data-bs-toggle="collapse" 
                                    data-bs-target="#stackTrace" aria-expanded="false" aria-controls="stackTrace">
                                Show Stack Trace
                            </button>
                            <div class="collapse mt-3" id="stackTrace">
                                <div class="card card-body bg-light text-start">
                                    <pre class="mb-0" style="white-space: pre-wrap; word-wrap: break-word;">
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
    </div>
</div>
