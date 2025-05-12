import { Component } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Router, RouterLink } from '@angular/router';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-register',
  standalone: true,
  imports: [CommonModule, FormsModule, RouterLink],
  templateUrl: './register.component.html',
  styleUrl: './register.component.css'
})
export class RegisterComponent {
  username: string = '';
  password: string = '';
  confirmPassword: string = '';
  errors: string[] = [];
  isLoading: boolean = false;

  constructor(private authService: AuthService, private router: Router) {}

  onSubmit() {
    this.errors = [];
    this.isLoading = true;

    // Validate form fields
    if (!this.username) {
      this.errors.push('Username is required');
    }
    if (!this.password) {
      this.errors.push('Password is required');
    }
    if (this.password !== this.confirmPassword) {
      this.errors.push('Passwords do not match');
    }
    if (this.password.length < 6) {
      this.errors.push('Password must be at least 6 characters long');
    }

    if (this.errors.length > 0) {
      this.isLoading = false;
      return;
    }

    this.authService.register(this.username, this.password).subscribe({
      next: (response) => {
        this.isLoading = false;
        this.router.navigate(['/']);
      },
      error: (error) => {
        this.isLoading = false;
        if (error.error?.errors && Array.isArray(error.error.errors)) {
          this.errors = error.error.errors;
        } else {
          this.errors.push(error.error?.message || 'Registration failed. Please try again.');
        }
      }
    });
  }
}
