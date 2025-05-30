# Picture Voting Application

A web application built with Java Servlets, JSP, and MySQL that allows users to upload pictures and vote on them.

## Features

- User registration and authentication
- Picture upload with title and description
- Voting system for pictures
- View top-rated pictures
- User dashboard to manage own pictures
- Responsive design with Bootstrap

## Prerequisites

- Java 11 or higher
- Maven 3.6 or higher
- MySQL 8.0 or higher
- A servlet container (Tomcat, Jetty, etc.)

## Setup Instructions

### 1. Database Setup

1. Install and start MySQL server
2. Create a database named `picture_voting_db`:
   ```sql
   CREATE DATABASE picture_voting_db;
   ```
3. Update database credentials in `src/main/resources/database.properties`:
   ```properties
   db.url=jdbc:mysql://localhost:3306/picture_voting_db
   db.user=your_username
   db.password=your_password
   ```

### 2. Build and Run

#### Option 1: Using Maven Jetty Plugin (Recommended for Development)

1. Navigate to the project directory:
   ```bash
   cd picture-voting-app
   ```

2. Clean and compile the project:
   ```bash
   mvn clean compile
   ```

3. Run with Jetty:
   ```bash
   mvn jetty:run
   ```

4. Access the application at: `http://localhost:8080/picture-voting`

#### Option 2: Deploy to Tomcat

1. Build the WAR file:
   ```bash
   mvn clean package
   ```

2. Copy the generated WAR file from `target/picture-voting-app.war` to your Tomcat `webapps` directory

3. Start Tomcat and access the application at: `http://localhost:8080/picture-voting-app`

## Application Structure

```
src/
├── main/
│   ├── java/
│   │   └── com/picturevoting/
│   │       ├── dao/           # Data Access Objects
│   │       ├── model/         # Entity classes
│   │       ├── servlet/       # Servlet controllers
│   │       └── util/          # Utility classes
│   ├── resources/
│   │   └── database.properties
│   └── webapp/
│       ├── WEB-INF/
│       │   ├── views/         # JSP templates
│       │   └── web.xml        # Web configuration
│       ├── resources/         # Static resources (CSS, JS, images)
│       └── uploads/           # Uploaded pictures directory
```

## Usage

1. **Register**: Create a new account with username, email, and password
2. **Login**: Sign in with your credentials
3. **Upload Pictures**: Click "Upload Picture" to add new images with title and description
4. **Vote**: Click the vote button on pictures uploaded by other users
5. **View Rankings**: See top-rated pictures on the home page
6. **Manage Pictures**: View and delete your own uploaded pictures

## Database Schema

The application automatically creates the following tables:

- `users`: User accounts
- `pictures`: Uploaded pictures with metadata
- `votes`: User votes on pictures

## Security Features

- Session-based authentication
- CSRF protection through proper form handling
- File upload validation
- SQL injection prevention with prepared statements
- XSS protection through proper output encoding

## Troubleshooting

### Common Issues

1. **Database Connection Error**: 
   - Verify MySQL is running
   - Check database credentials in `database.properties`
   - Ensure the database `picture_voting_db` exists

2. **File Upload Issues**:
   - Ensure the `uploads` directory exists and is writable
   - Check file size limits in servlet configuration

3. **404 Errors**:
   - Verify the context path matches your deployment
   - Check servlet mappings in web.xml

### Logs

Check the application server logs for detailed error information:
- Jetty: Console output
- Tomcat: `logs/catalina.out`

## Development

To modify the application:

1. Make changes to Java files in `src/main/java`
2. Update JSP templates in `src/main/webapp/WEB-INF/views`
3. Modify styles in `src/main/webapp/resources/css/style.css`
4. Update JavaScript in `src/main/webapp/resources/js/main.js`

The Jetty plugin supports hot reloading for most changes during development.

## License

This project is for educational purposes. 