document.addEventListener("DOMContentLoaded", function () {
  const applyFiltersButton = document.getElementById("apply_filters");
  const newsList = document.getElementById("news_list");
  const activeFilters = document.getElementById("active_filters");
  let currentFilters = {};
  let filterHistory = [];
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

  function generateFilterText(filters, isActive = true) {
    const filterParts = [];

    if (filters.date_from) {
      filterParts.push(`From: ${filters.date_from}`);
    }
    if (filters.date_to) {
      filterParts.push(`To: ${filters.date_to}`);
    }
    if (filters.category) {
      filterParts.push(`Category: ${filters.category}`);
    }

    if (filterParts.length === 0) {
      return isActive ? "No active filters" : "No filters";
    } else {
      return filterParts.join(", ");
    }
  }

  function updateActiveFilters() {
    activeFilters.innerHTML = ""; // Clear existing content

    // Create active filters section
    const activeSection = document.createElement("div");
    activeSection.className = "filter-section active-section";
    const activeHeading = document.createElement("h3");
    activeHeading.textContent = "Active filters: ";
    activeSection.appendChild(activeHeading);

    const activeText = document.createElement("span");
    activeText.textContent = generateFilterText(currentFilters);
    activeSection.appendChild(activeText);

    activeFilters.appendChild(activeSection);

    // Add previous filters if there are any
    if (filterHistory.length > 0) {
      const previousSection = document.createElement("div");
      previousSection.className = "filter-section previous-section";
      const previousHeading = document.createElement("h3");
      previousHeading.textContent = "Previous filters:";
      previousSection.appendChild(previousHeading);

      // Create list for previous filters
      const previousList = document.createElement("ul");
      filterHistory.forEach((filter) => {
        const listItem = document.createElement("li");
        listItem.textContent = generateFilterText(filter, false);
        previousList.appendChild(listItem);
      });

      previousSection.appendChild(previousList);
      activeFilters.appendChild(previousSection);
    }
  }

  function formatDate(dateString) {
    const options = { year: "numeric", month: "long", day: "numeric" };
    return new Date(dateString).toLocaleDateString(undefined, options);
  }

  function loadNews() {
    if (isLoading) return;

    showLoading();
    const params = new URLSearchParams(currentFilters);
    const url = `api/get_news.php?${params}`;
    console.log("Fetching news from:", url);

    fetch(url)
      .then((response) => {
        console.log("Response status:", response.status);
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.text().then((text) => {
          console.log("Raw response:", text);
          try {
            return JSON.parse(text);
          } catch (e) {
            console.error("JSON parse error:", e);
            throw new Error("Invalid JSON response");
          }
        });
      })
      .then((data) => {
        console.log("Parsed data:", data);
        hideLoading();
        newsList.innerHTML = "";

        if (!data || data.length === 0) {
          newsList.innerHTML =
            '<div class="no-news">No news found matching your criteria.</div>';
          return;
        }

        data.forEach((news) => {
          const newsItem = document.createElement("div");
          newsItem.className = "news-item";
          newsItem.innerHTML = `
                        <h2>${news.title}</h2>
                        <div class="news-meta">
                            <span class="producer">By: ${news.producer}</span>
                            <span class="category">Category: ${
                              news.category
                            }</span>
                            <span class="importance">Importance: ${
                              news.importance
                            }</span>
                            <span class="date">${formatDate(
                              news.created_at
                            )}</span>
                        </div>
                        <div class="news-content">${news.content}</div>
                        ${
                          news.can_edit
                            ? `
                            <div class="news-actions">
                                <a href="edit_news.php?id=${news.id}" class="button">Edit</a>
                                <button onclick="deleteNews(${news.id})" class="button delete">Delete</button>
                            </div>
                        `
                            : ""
                        }
                    `;
          newsList.appendChild(newsItem);
        });
      })
      .catch((error) => {
        hideLoading();
        showError(`Error loading news: ${error.message}`);
        console.error("Error loading news:", error);
      });
  }

  function deleteNews(id) {
    if (!confirm("Are you sure you want to delete this news item?")) {
      return;
    }

    fetch("api/delete_news.php", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ id: id }),
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.json();
      })
      .then((data) => {
        if (data.success) {
          loadNews();
        } else {
          throw new Error(data.message || "Failed to delete news");
        }
      })
      .catch((error) => {
        alert(`Error deleting news: ${error.message}`);
        console.error("Error deleting news:", error);
      });
  }

  // Make deleteNews function available globally
  window.deleteNews = deleteNews;

  // Event Listeners
  applyFiltersButton.addEventListener("click", function () {
    // Save current filters to history before updating
    if (Object.keys(currentFilters).some((key) => currentFilters[key])) {
      // Only add to history if there were actual filters
      filterHistory.unshift({ ...currentFilters }); // Add copy of current filters to beginning of history
      // Limit history to last 5 filter sets
      if (filterHistory.length > 5) {
        filterHistory.pop();
      }
    }

    // Update current filters
    currentFilters = {
      date_from: document.getElementById("date_from").value,
      date_to: document.getElementById("date_to").value,
      category: document.getElementById("category").value,
    };

    updateActiveFilters();
    loadNews();
  });

  // Load initial news
  loadNews();
  updateActiveFilters();
});
