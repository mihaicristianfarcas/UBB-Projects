<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>

<div class="mb-5">
    <div class="d-flex justify-content-between align-items-center mb-4">
        <h2>Top Pictures</h2>
        <div class="d-flex align-items-center">
            <form action="${pageContext.request.contextPath}/home" method="get" class="d-flex">
                <label for="top" class="form-label me-2 my-0">Show top</label>
                <input type="number" id="top" name="top" min="1" max="50" value="${topCount}" class="form-control me-2" style="width: 70px;">
                <button type="submit" class="btn btn-sm btn-outline-secondary">Go</button>
            </form>
        </div>
    </div>
    
    <c:choose>
        <c:when test="${not empty topPictures}">
            <div class="row row-cols-1 row-cols-md-2 row-cols-lg-3 g-4">
                <c:forEach items="${topPictures}" var="picture">
                    <div class="col">
                        <div class="card h-100 shadow-sm">
                            <img src="${pageContext.request.contextPath}/${picture.filePath}" 
                                 class="card-img-top" alt="${picture.title}" style="height: 200px; object-fit: cover;">
                            <div class="card-body">
                                <h5 class="card-title">${picture.title}</h5>
                                <p class="card-text text-muted">
                                    <small>By ${picture.uploaderUsername}</small>
                                </p>
                                <p class="card-text">
                                    <c:choose>
                                        <c:when test="${fn:length(picture.description) > 100}">
                                            ${fn:substring(picture.description, 0, 100)}...
                                        </c:when>
                                        <c:otherwise>
                                            ${picture.description}
                                        </c:otherwise>
                                    </c:choose>
                                </p>
                            </div>
                            <div class="card-footer bg-transparent d-flex justify-content-between align-items-center">
                                <div>
                                    <span class="badge bg-primary">
                                        <i class="bi bi-star-fill me-1"></i> ${picture.voteCount}
                                    </span>
                                </div>
                                <c:if test="${sessionScope.user.id ne picture.userId}">
                                    <form action="${pageContext.request.contextPath}/vote" method="post" class="d-inline-flex align-items-center">
                                        <input type="hidden" name="pictureId" value="${picture.id}">
                                        <div class="input-group input-group-sm me-2" style="width: 100px;">
                                            <input type="number" name="voteValue" min="1" max="10" value="5" 
                                                   class="form-control form-control-sm text-center" required>
                                        </div>
                                        <button type="submit" class="btn btn-sm btn-outline-primary">
                                            <i class="bi bi-star-fill"></i> Vote
                                        </button>
                                    </form>
                                </c:if>
                            </div>
                        </div>
                    </div>
                </c:forEach>
            </div>
        </c:when>
        <c:otherwise>
            <div class="alert alert-info">No pictures have been uploaded yet. Be the first to upload one!</div>
        </c:otherwise>
    </c:choose>
</div>

<c:if test="${not empty userPictures}">
    <div class="mt-5">
        <h2 class="mb-4">My Pictures</h2>
        <div class="row row-cols-1 row-cols-md-2 row-cols-lg-3 g-4">
            <c:forEach items="${userPictures}" var="picture">
                <div class="col">
                    <div class="card h-100 shadow-sm">
                        <img src="${pageContext.request.contextPath}/${picture.filePath}" 
                             class="card-img-top" alt="${picture.title}" style="height: 200px; object-fit: cover;">
                        <div class="card-body">
                            <h5 class="card-title">${picture.title}</h5>
                            <p class="card-text">
                                <c:choose>
                                    <c:when test="${fn:length(picture.description) > 100}">
                                        ${fn:substring(picture.description, 0, 100)}...
                                    </c:when>
                                    <c:otherwise>
                                        ${picture.description}
                                    </c:otherwise>
                                </c:choose>
                            </p>
                        </div>
                        <div class="card-footer bg-transparent d-flex justify-content-between align-items-center">
                            <span class="badge bg-primary">
                                <i class="bi bi-star-fill me-1"></i> ${picture.voteCount}
                            </span>
                            <form action="${pageContext.request.contextPath}/delete-picture" method="post" class="d-inline" 
                                  onsubmit="return confirm('Are you sure you want to delete this picture? This action cannot be undone.');">
                                <input type="hidden" name="pictureId" value="${picture.id}">
                                <button type="submit" class="btn btn-sm btn-outline-danger">
                                    <i class="bi bi-trash"></i> Delete
                                </button>
                            </form>
                        </div>
                    </div>
                </div>
            </c:forEach>
        </div>
    </div>
</c:if>
