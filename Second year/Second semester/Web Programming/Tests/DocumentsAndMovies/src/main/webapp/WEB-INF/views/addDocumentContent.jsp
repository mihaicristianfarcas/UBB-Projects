<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<form class="mt-8 space-y-6" action="${pageContext.request.contextPath}/add_document" method="post">
    <div class="rounded-md shadow-sm -space-y-px flex flex-col items-center justify-center gap-4">
        <div>
            <label for="name" class="block font-medium text-gray-700">Document Name</label>
            <input id="name" name="name" type="text" required minlength="3" maxlength="50"
                value="${param.name != null ? param.name : ''}"
                class="appearance-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-md focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm">
            <p class="mt-1 text-sm text-gray-500">3-50 characters</p>
        </div>

        <div>
            <label for="contents" class="block text-sm font-medium text-gray-700">Content</label>
            <input id="contents" name="contents" type="text"
                autocomplete="new-contents" required minlength="1" maxlength="200"
                class="appearance-none relative block w-full px-30 py-20 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-md focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm">
            <p class="mt-1 text-sm text-gray-500">1-200 characters</p>
        </div>
    </div>
    <div>
        <button type="submit"
            class="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-blue-600 hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500">
            Add Document
        </button>
    </div>
</form>