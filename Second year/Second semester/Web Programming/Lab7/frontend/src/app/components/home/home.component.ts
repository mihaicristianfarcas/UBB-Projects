import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { RouterLink } from '@angular/router';
import { ApiService, News, NewsFilters } from '../../api.service';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-home',
  standalone: true,
  imports: [CommonModule, FormsModule, RouterLink],
  templateUrl: './home.component.html',
  styleUrl: './home.component.css'
})
export class HomeComponent implements OnInit {
  news: News[] = [];
  categories: string[] = [];
  filters: NewsFilters = {}; // Filters in the form
  appliedFilters: NewsFilters = {}; // Filters actually applied to the news
  loading: boolean = false;
  error: string = '';
  filterHistory: NewsFilters[] = [];
  
  constructor(private apiService: ApiService, public authService: AuthService) {}

  ngOnInit(): void {
    this.loadNews();
    this.loadCategories();
  }

  loadNews(): void {
    this.loading = true;
    this.apiService.getNews(this.appliedFilters).subscribe({
      next: (data) => {
        this.news = data;
        this.loading = false;
      },
      error: (error) => {
        this.error = 'Failed to load news';
        this.loading = false;
        console.error('Error loading news:', error);
      }
    });
  }

  loadCategories(): void {
    this.apiService.getCategories().subscribe({
      next: (data) => {
        this.categories = data;
      },
      error: (error) => {
        console.error('Error loading categories:', error);
      }
    });
  }

  applyFilters(): void {
    // Copy current form filters to applied filters
    this.appliedFilters = {...this.filters};
    
    // Add current filters to history only if they're not empty and not already in history
    if (this.hasFiltersSet(this.filters)) {
      // Create a copy of current filters
      const currentFilter = {...this.filters};
      
      // Check if this filter is already in history by comparing values
      const filterExists = this.filterHistory.some(filter => 
        filter.category === currentFilter.category &&
        filter.date_from === currentFilter.date_from &&
        filter.date_to === currentFilter.date_to
      );
      
      // Only add to history if it's a new filter
      if (!filterExists) {
        this.filterHistory.unshift(currentFilter);
        if (this.filterHistory.length > 5) {
          this.filterHistory.pop(); // Keep only last 5 filters
        }
      }
    }
    
    this.loadNews();
  }

  deleteNews(id: number): void {
    if (confirm('Are you sure you want to delete this news item?')) {
      this.apiService.deleteNews(id).subscribe({
        next: () => {
          this.loadNews();
        },
        error: (error) => {
          console.error('Error deleting news:', error);
          alert('Failed to delete news');
        }
      });
    }
  }

  formatDate(dateString: string): string {
    return new Date(dateString).toLocaleDateString('en-US', {
      year: 'numeric',
      month: 'long',
      day: 'numeric'
    });
  }

  hasFiltersSet(filterObj: NewsFilters): boolean {
    return !!filterObj.category || !!filterObj.date_from || !!filterObj.date_to;
  }

  hasActiveFilters(): boolean {
    return this.hasFiltersSet(this.appliedFilters);
  }

  clearFilters(): void {
    this.filters = {};
    this.appliedFilters = {};
    this.loadNews();
  }
}
