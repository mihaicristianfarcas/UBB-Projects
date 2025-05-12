import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Router, RouterLink } from '@angular/router';
import { ApiService, News } from '../../api.service';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-add-news',
  standalone: true,
  imports: [CommonModule, FormsModule, RouterLink],
  templateUrl: './add-news.component.html',
  styleUrl: './add-news.component.css'
})
export class AddNewsComponent implements OnInit {
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
    private router: Router
  ) {}

  ngOnInit(): void {
    // Check authentication state
    if (!this.authService.isLoggedIn) {
      this.router.navigate(['/login']);
      return;
    }

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

  onSubmit(): void {
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
    this.apiService.addNews(this.news).subscribe({
      next: (response) => {
        this.isLoading = false;
        this.router.navigate(['/']);
      },
      error: (error) => {
        this.isLoading = false;
        if (error.error?.errors && Array.isArray(error.error.errors)) {
          this.errors = error.error.errors;
        } else {
          this.errors.push(error.error?.message || 'Failed to add news. Please try again.');
        }
      }
    });
  }
}
