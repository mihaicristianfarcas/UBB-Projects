<div class="min-h-full flex items-center justify-center py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-md w-full space-y-8">
        <div class="bg-white py-8 px-4 shadow-xl rounded-lg sm:px-10 text-center">
            <div class="text-6xl text-red-500 mb-4">
                <i class="fas fa-exclamation-circle"></i>
            </div>
            <h1 class="text-8xl font-bold text-gray-400 mb-4">500</h1>
            <h2 class="text-2xl font-semibold text-gray-900 mb-4">Oops! Something went wrong.</h2>
            <div class="mt-8 space-x-4">
                <a href="${pageContext.request.contextPath}/home" 
                   class="inline-flex items-center px-6 py-3 border border-transparent text-base font-medium rounded-md text-white bg-blue-600 hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
                    <i class="fas fa-home mr-2"></i> Go to Home
                </a>
                <button onclick="window.history.back()" 
                        class="inline-flex items-center px-6 py-3 border border-gray-300 text-base font-medium rounded-md text-gray-700 bg-white hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
                    <i class="fas fa-arrow-left mr-2"></i> Go Back
                </button>
            </div>
        </div>
    </div>
</div>
