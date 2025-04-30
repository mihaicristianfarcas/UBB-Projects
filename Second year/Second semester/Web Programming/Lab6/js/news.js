document.addEventListener('DOMContentLoaded', function() {
    const applyFiltersButton = document.getElementById('apply_filters');
    const newsList = document.getElementById('news_list');
    const activeFilters = document.getElementById('active_filters');
    let currentFilters = {};
    let isLoading = false;

    function showLoading() {
        isLoading = true;
        newsList.innerHTML = '<div class="loading">Loading news...</div>';
    }

    function hideLoading() {
        isLoading = false;
    }

    function showError(message) {
        newsList.innerHTML = `<div class="error">${message}</div>`;
    }

    function updateActiveFilters() {
        let filterText = 'Active filters: ';
        const filters = [];
        
        if (currentFilters.date_from) {
            filters.push(`From: ${currentFilters.date_from}`);
        }
        if (currentFilters.date_to) {
            filters.push(`To: ${currentFilters.date_to}`);
        }
        if (currentFilters.category) {
            filters.push(`Category: ${currentFilters.category}`);
        }

        if (filters.length === 0) {
            filterText = 'No active filters';
        } else {
            filterText += filters.join(', ');
        }

        activeFilters.textContent = filterText;
    }

    function formatDate(dateString) {
        const options = { year: 'numeric', month: 'long', day: 'numeric' };
        return new Date(dateString).toLocaleDateString(undefined, options);
    }

    function loadNews() {
        if (isLoading) return;
        
        showLoading();
        const params = new URLSearchParams(currentFilters);
        const url = `api/get_news.php?${params}`;
        console.log('Fetching news from:', url);
        
        fetch(url)
            .then(response => {
                console.log('Response status:', response.status);
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                return response.text().then(text => {
                    console.log('Raw response:', text);
                    try {
                        return JSON.parse(text);
                    } catch (e) {
                        console.error('JSON parse error:', e);
                        throw new Error('Invalid JSON response');
                    }
                });
            })
            .then(data => {
                console.log('Parsed data:', data);
                hideLoading();
                newsList.innerHTML = '';
                
                if (!data || data.length === 0) {
                    newsList.innerHTML = '<div class="no-news">No news found matching your criteria.</div>';
                    return;
                }

                data.forEach(news => {
                    const newsItem = document.createElement('div');
                    newsItem.className = 'news-item';
                    newsItem.innerHTML = `
                        <h2>${news.title}</h2>
                        <div class="news-meta">
                            <span class="producer">By: ${news.producer}</span>
                            <span class="category">Category: ${news.category}</span>
                            <span class="date">${formatDate(news.created_at)}</span>
                        </div>
                        <div class="news-content">${news.content}</div>
                        ${news.can_edit ? `
                            <div class="news-actions">
                                <a href="edit_news.php?id=${news.id}" class="button">Edit</a>
                                <button onclick="deleteNews(${news.id})" class="button delete">Delete</button>
                            </div>
                        ` : ''}
                    `;
                    newsList.appendChild(newsItem);
                });
            })
            .catch(error => {
                hideLoading();
                showError(`Error loading news: ${error.message}`);
                console.error('Error loading news:', error);
            });
    }

    function deleteNews(id) {
        if (!confirm('Are you sure you want to delete this news item?')) {
            return;
        }

        fetch('api/delete_news.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ id: id })
        })
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            return response.json();
        })
        .then(data => {
            if (data.success) {
                loadNews();
            } else {
                throw new Error(data.message || 'Failed to delete news');
            }
        })
        .catch(error => {
            alert(`Error deleting news: ${error.message}`);
            console.error('Error deleting news:', error);
        });
    }

    // Event Listeners
    applyFiltersButton.addEventListener('click', function() {
        currentFilters = {
            date_from: document.getElementById('date_from').value,
            date_to: document.getElementById('date_to').value,
            category: document.getElementById('category').value
        };
        updateActiveFilters();
        loadNews();
    });

    // Auto-load news when filters change
    document.getElementById('date_from').addEventListener('change', loadNews);
    document.getElementById('date_to').addEventListener('change', loadNews);
    document.getElementById('category').addEventListener('change', loadNews);

    // Load initial news
    loadNews();
}); 