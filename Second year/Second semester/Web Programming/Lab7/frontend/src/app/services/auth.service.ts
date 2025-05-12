import { Injectable, PLATFORM_ID, Inject } from '@angular/core';
import { BehaviorSubject, Observable, tap } from 'rxjs';
import { ApiService } from '../api.service';
import { isPlatformBrowser } from '@angular/common';

interface AuthUser {
  id: number;
  username: string;
}

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private currentUserSubject = new BehaviorSubject<AuthUser | null>(null);
  public currentUser$ = this.currentUserSubject.asObservable();
  private isBrowser: boolean;

  constructor(
    private apiService: ApiService,
    @Inject(PLATFORM_ID) platformId: Object
  ) {
    this.isBrowser = isPlatformBrowser(platformId);
    
    // Check if user session exists - only in browser environment
    if (this.isBrowser) {
      const storedUser = localStorage.getItem('user');
      if (storedUser) {
        this.currentUserSubject.next(JSON.parse(storedUser));
      }
    }
  }

  login(username: string, password: string): Observable<any> {
    return this.apiService.login(username, password).pipe(
      tap(response => {
        if (response && response.success && response.user) {
          if (this.isBrowser) {
            localStorage.setItem('user', JSON.stringify(response.user));
          }
          this.currentUserSubject.next(response.user);
        }
      })
    );
  }

  register(username: string, password: string): Observable<any> {
    return this.apiService.register(username, password).pipe(
      tap(response => {
        if (response && response.success && response.user) {
          if (this.isBrowser) {
            localStorage.setItem('user', JSON.stringify(response.user));
          }
          this.currentUserSubject.next(response.user);
        }
      })
    );
  }

  logout(): Observable<any> {
    return this.apiService.logout().pipe(
      tap(() => {
        if (this.isBrowser) {
          localStorage.removeItem('user');
        }
        this.currentUserSubject.next(null);
      })
    );
  }

  get isLoggedIn(): boolean {
    return !!this.currentUserSubject.value;
  }

  get currentUser(): AuthUser | null {
    return this.currentUserSubject.value;
  }
}
