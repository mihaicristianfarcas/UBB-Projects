import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

export interface News {
  id: number;
  title: string;
  content: string;
  category: string;
  importance: number;
  created_at: string;
  updated_at: string;
  user_id: number;
  producer: string;
  can_edit: boolean;
}

export interface NewsFilters {
  date_from?: string;
  date_to?: string;
  category?: string;
}

export interface User {
  username: string;
  password: string;
}

@Injectable({
  providedIn: 'root'
})
export class ApiService {
  private apiUrl = 'http://localhost:8000';

  constructor(private http: HttpClient) { }

  // Authentication
  login(username: string, password: string): Observable<any> {
    const formData = new FormData();
    formData.append('username', username);
    formData.append('password', password);
    return this.http.post(`${this.apiUrl}/login.php`, formData, { withCredentials: true });
  }

  register(username: string, password: string): Observable<any> {
    const formData = new FormData();
    formData.append('username', username);
    formData.append('password', password);
    return this.http.post(`${this.apiUrl}/register.php`, formData, { withCredentials: true });
  }

  logout(): Observable<any> {
    return this.http.get(`${this.apiUrl}/logout.php`, { withCredentials: true });
  }

  // News operations
  getNews(filters?: NewsFilters): Observable<News[]> {
    let url = `${this.apiUrl}/api/get_news.php`;
    
    if (filters) {
      const params = new URLSearchParams();
      if (filters.date_from) params.append('date_from', filters.date_from);
      if (filters.date_to) params.append('date_to', filters.date_to);
      if (filters.category) params.append('category', filters.category);
      url += `?${params.toString()}`;
    }
    
    return this.http.get<News[]>(url, { withCredentials: true });
  }

  addNews(news: Partial<News>): Observable<any> {
    const formData = new FormData();
    formData.append('title', news.title || '');
    formData.append('content', news.content || '');
    formData.append('category', news.category || '');
    formData.append('importance', news.importance?.toString() || '');
    
    return this.http.post(`${this.apiUrl}/add_news.php`, formData, { withCredentials: true });
  }

  updateNews(id: number, news: Partial<News>): Observable<any> {
    const formData = new FormData();
    formData.append('title', news.title || '');
    formData.append('content', news.content || '');
    formData.append('category', news.category || '');
    formData.append('importance', news.importance?.toString() || '');
    
    return this.http.post(`${this.apiUrl}/edit_news.php?id=${id}`, formData, { withCredentials: true });
  }

  deleteNews(id: number): Observable<any> {
    const options = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json',
      }),
      withCredentials: true,
      body: { id }
    };
    
    return this.http.post(`${this.apiUrl}/api/delete_news.php`, { id }, { withCredentials: true });
  }

  // Get categories for filter
  getCategories(): Observable<string[]> {
    return this.http.get<string[]>(`${this.apiUrl}/api/categories.php`, { withCredentials: true });
  }
}
