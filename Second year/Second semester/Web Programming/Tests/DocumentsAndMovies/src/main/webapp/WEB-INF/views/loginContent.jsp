<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<div class="min-h-full flex items-center justify-center py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-md w-full space-y-8">
        <div class="bg-white py-8 px-4 shadow rounded-lg sm:px-10">
            <div class="text-center">
                <h2 class="mt-6 text-3xl font-extrabold text-gray-900">Sign in to your account</h2>
            </div>
            
            <form class="mt-8 space-y-6" action="${pageContext.request.contextPath}/login" method="post">
                <div class="rounded-md shadow-sm -space-y-px flex flex-row items-center justify-center gap-4">
                    <div>
                        <label for="name" class="block text-sm font-medium text-gray-700">Author Name</label>
                        <input id="name" name="name" type="text" required minlength="3" maxlength="50"
                            value="${param.name != null ? param.name : ''}"
                            class="appearance-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-md focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm">
                        <p class="mt-1 text-sm text-gray-500">3-50 characters</p>
                    </div>
                
                    <div>
                        <label for="document_or_movie_name_or_id" class="block text-sm font-medium text-gray-700">Document/Movie Name/ID</label>
                        <input id="document_or_movie_name_or_id" name="document_or_movie_name_or_id" type="text"
                            autocomplete="new-document_or_movie_name_or_id" required minlength="1" maxlength="50"
                            class="appearance-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-md focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm">
                            <p class="mt-1 text-sm text-gray-500">1-50 characters</p>
                    </div>
                </div>
                <div>
                    <button type="submit"
                            class="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-blue-600 hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
                        Sign in
                    </button>
                </div>
            </form>
        </div>
    </div>
</div>
