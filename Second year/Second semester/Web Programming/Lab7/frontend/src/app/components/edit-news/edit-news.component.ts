import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Router, RouterLink, ActivatedRoute } from '@angular/router';
import { ApiService, News } from '../../api.service';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-edit-news',
  standalone: true,
  imports: [CommonModule, FormsModule, RouterLink],
  templateUrl: './edit-news.component.html',
  styleUrl: './edit-news.component.css'
})
export class EditNewsComponent implements OnInit {
  newsId: number | null = null;
  news: Partial<News> = {
    title: '',
    content: '',
    category: '',
    importance: 1
  };
  errors: string[] = [];
  isLoading: boolean = false;
  categories: string[] = [];

  constructor(
    private apiService: ApiService, 
    private authService: AuthService,
    private router: Router,
    private route: ActivatedRoute
  ) {}

  ngOnInit(): void {
    // Check authentication state
    if (!this.authService.isLoggedIn) {
      this.router.navigate(['/login']);
      return;
    }

    // Get news ID from route
    this.route.params.subscribe(params => {
      if (params['id']) {
        this.newsId = parseInt(params['id']);
        this.loadNews(this.newsId);
      } else {
        this.router.navigate(['/']);
      }
    });

    // Load categories
    this.apiService.getCategories().subscribe({
      next: (data) => {
        this.categories = data;
      },
      error: (error) => {
        console.error('Error loading categories:', error);
      }
    });
  }

  loadNews(id: number): void {
    this.isLoading = true;
    this.apiService.getNews().subscribe({
      next: (data) => {
        const newsItem = data.find(item => item.id === id);
        if (newsItem) {
          if (!newsItem.can_edit) {
            // User cannot edit this news item
            this.router.navigate(['/']);
            return;
          }
          this.news = { ...newsItem };
        } else {
          this.router.navigate(['/']);
        }
        this.isLoading = false;
      },
      error: (error) => {
        this.isLoading = false;
        this.errors.push('Failed to load news item');
        console.error('Error loading news:', error);
      }
    });
  }

  onSubmit(): void {
    if (!this.newsId) {
      this.router.navigate(['/']);
      return;
    }

    this.errors = [];
    this.isLoading = true;

    // Validate fields
    if (!this.news.title) {
      this.errors.push('Title is required');
    }
    if (!this.news.content) {
      this.errors.push('Content is required');
    }
    if (!this.news.category) {
      this.errors.push('Category is required');
    }

    if (this.errors.length > 0) {
      this.isLoading = false;
      return;
    }

    // Submit the news
    this.apiService.updateNews(this.newsId, this.news).subscribe({
      next: (response) => {
        this.isLoading = false;
        this.router.navigate(['/']);
      },
      error: (error) => {
        this.isLoading = false;
        if (error.error?.errors && Array.isArray(error.error.errors)) {
          this.errors = error.error.errors;
        } else {
          this.errors.push(error.error?.message || 'Failed to update news. Please try again.');
        }
      }
    });
  }
}
