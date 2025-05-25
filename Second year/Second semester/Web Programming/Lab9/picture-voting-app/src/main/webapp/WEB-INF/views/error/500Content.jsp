<div class="container text-center py-5">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card shadow">
                <div class="card-body p-5">
                    <div class="display-1 text-danger mb-4">
                        <i class="bi bi-exclamation-octagon"></i> 500
                    </div>
                    <h1 class="h2 mb-3">Oops! Something went wrong.</h1>
                    <p class="h4 text-muted font-weight-normal mb-4">We're experiencing some trouble on our end. Please try again later.</p>
                    <div class="mt-5">
                        <a href="${pageContext.request.contextPath}/home" class="btn btn-primary me-3">
                            <i class="bi bi-house-door"></i> Go to Home
                        </a>
                        <button onclick="window.history.back()" class="btn btn-outline-secondary">
                            <i class="bi bi-arrow-left"></i> Go Back
                        </button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
