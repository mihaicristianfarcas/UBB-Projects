# Exam Requirement: Real Estate Sales App
A group of users is managing their real estate sales using a mobile application. Users can track property sales, rentals, and leases.

On the server side, at least the following details are maintained:
- `id`: The unique identifier for the transaction. Integer value greater than zero.
- `date`: The date when the transaction occurred. A string in the format "YYYY-MM-DD".
- `amount`: The sale price or rent amount. A decimal value.
- `type`: The type of transaction (e.g., sale, lease, rent). A string of characters.
- `category`: The category of the property (e.g., house, condo, land). A string of characters.
- `description`: A description of the property. A string of characters.

The application should provide at least the following features:

## Main Section (Separate Screen/Activity)
> **Note:** Each feature in this section should be implemented in a separate screen unless otherwise specified.

- A. **(1p) View the list of sales**: Using the `GET /sales` call, users can retrieve all their sales records. If offline, the app will display an offline message and provide a retry option. Once retrieved, the data should be available on the device, regardless of whether online or offline.
- B. **(2p) View Sale Details**: By selecting a sale from the list, the user can view its details. The `GET /sale/:id` call will retrieve specific details. Once retrieved, the data should be available on the device, regardless of whether online or offline.
- C. **(1p) Add a new sale**: Users can create a new sale transaction using the `POST /sale` call by specifying all details. This feature is available online only.
- D. **(1p) Delete a sale**: Users can delete a sale record using the `DELETE /sale/:id` call by selecting it from the list. This feature is available online only.

## Reports Section (Separate Screen/Activity)
> **Note:** This section uses different API endpoints than the Main section.

**(1p) Monthly Sales Analysis**: Using the `GET /allSales` call, the app will retrieve all records and compute the list of monthly totals, displayed in descending order.

## Insights Section (Separate Screen/Activity)
> **Note:** This section uses different API endpoints than the Main section.

**(1p) Top Property Categories**: View the top 3 property categories by volume. Using the same `GET /allSales` call, the app will compute and display the top 3 categories and their total amounts in descending order.

## Additional Features
- **(1p) WebSocket Notifications**: When a new sale is added, the server will use a WebSocket channel to send the details to all connected clients. The app will display the received data in human-readable form (e.g., as a toast, snackbar, or dialog).
- **(0.5p) Progress Indicator**: A progress indicator will be displayed during server operations.
- **(0.5p) Error Handling & Logging**: Any server interaction errors will be displayed using a toast or snackbar, and all interactions (server or DB) will log a message.

## Server Info
- Location: `./server`
- Install: `npm install`
- Run: `npm start`
- Port: 2626
