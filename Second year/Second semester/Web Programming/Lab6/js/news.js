document.addEventListener('DOMContentLoaded', function() {
    const applyFiltersBtn = document.getElementById('apply_filters');
    const newsList = document.getElementById('news_list');
    const activeFilters = document.getElementById('active_filters');
    let currentFilters = {};

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

    function loadNews() {
        const params = new URLSearchParams(currentFilters);
        fetch(`api/get_news.php?${params}`)
            .then(response => response.json())
            .then(data => {
                newsList.innerHTML = '';
                data.forEach(news => {
                    const newsItem = document.createElement('div');
                    newsItem.className = 'news-item';
                    newsItem.innerHTML = `
                        <h2>${news.title}</h2>
                        <div class="news-meta">
                            <span class="producer">By: ${news.producer}</span>
                            <span class="category">Category: ${news.category}</span>
                            <span class="date">${news.created_at}</span>
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
            .catch(error => console.error('Error loading news:', error));
    }

    function deleteNews(id) {
        if (confirm('Are you sure you want to delete this news item?')) {
            fetch('api/delete_news.php', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ id: id })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    loadNews();
                } else {
                    alert('Error deleting news: ' + data.message);
                }
            })
            .catch(error => console.error('Error deleting news:', error));
        }
    }

    applyFiltersBtn.addEventListener('click', function() {
        currentFilters = {
            date_from: document.getElementById('date_from').value,
            date_to: document.getElementById('date_to').value,
            category: document.getElementById('category').value
        };
        updateActiveFilters();
        loadNews();
    });

    // Load initial news
    loadNews();
}); 