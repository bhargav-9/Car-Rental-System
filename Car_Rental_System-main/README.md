# 🚗 Car Rental System

A full-stack web application for managing a car rental business, built with **React**, **Node.js/Express**, and **SQLite**. The system supports two roles — **Customer** and **Manager** — each with a dedicated dashboard and set of features.

---

## ✨ Features

### Customer
- Register and log in securely
- Browse all available cars with pricing
- Rent a car by specifying the number of days
- Return a car and report its condition
- Pay outstanding fines
- View personal profile, record score, and fine dues

### Manager
- Log in to a dedicated manager dashboard
- Add new cars to the fleet (model + daily rental price)
- Delete cars from the system (only if not currently rented)
- View all cars with their current status, condition, and who rented them

---

## 🏗️ Tech Stack

| Layer     | Technology                        |
|-----------|-----------------------------------|
| Frontend  | React 18 (Vite) + Framer Motion + Axios |
| Backend   | Node.js + Express.js              |
| Database  | SQLite (`database.sqlite`)        |
| Styling   | Vanilla CSS (glassmorphic UI)     |

---

## 📁 Project Structure

```
Car_Rental_System-main/
├── backend/
│   ├── server.js          # All API routes (auth, cars, rentals, users)
│   ├── database.js        # SQLite connection & table initialization
│   └── database.sqlite    # Persistent database file
└── frontend/
    ├── public/
    └── src/
        ├── pages/
        │   ├── Login.jsx              # Login & Register form
        │   ├── Dashboard.jsx          # Customer dashboard
        │   └── ManagerDashboard.jsx   # Manager dashboard
        ├── components/
        │   └── Navbar.jsx
        ├── App.jsx                    # Root component & role-based routing
        ├── App.css
        └── index.css
```

---

## 🗄️ Database Schema

### `users`
| Column    | Type    | Description                        |
|-----------|---------|------------------------------------|
| id        | INTEGER | Primary key                        |
| name      | TEXT    | Username                           |
| password  | TEXT    | Password                           |
| role      | TEXT    | `Customer` or `Manager`            |
| fine_due  | INTEGER | Outstanding fine amount (₹)        |
| record    | INTEGER | Trust score (starts at 10)         |
| mobile    | TEXT    | Mobile number                      |
| status    | TEXT    | `Existing` or `Deleted`            |

### `cars`
| Column       | Type    | Description                         |
|--------------|---------|-------------------------------------|
| id           | INTEGER | Primary key                         |
| model        | TEXT    | Car model name                      |
| condition    | TEXT    | `Good` or `Damaged`                 |
| due_date     | TEXT    | Expected return date                |
| rental_price | INTEGER | Daily rental price (₹)              |
| status       | TEXT    | `Available`, `In_Use`, or `Deleted` |
| rented_by    | INTEGER | FK → users.id                       |

### `rentals`
| Column      | Type    | Description                  |
|-------------|---------|------------------------------|
| id          | INTEGER | Primary key                  |
| car_id      | INTEGER | FK → cars.id                 |
| user_id     | INTEGER | FK → users.id                |
| start_date  | TEXT    | Rental start date            |
| end_date    | TEXT    | Return date (null if active) |
| total_price | INTEGER | Total cost (₹)               |
| status      | TEXT    | `Active` or `Returned`       |

---

## ⚙️ Business Rules

| Rule | Detail |
|------|--------|
| Fine limit | If `fine_due > ₹50,000`, customer cannot rent until paid |
| Record limit | If `record ≤ 0`, customer cannot rent a car |
| Late return fine | `1.5 × daily rental price × number of late days` |
| Damage fine | ₹5,000 charged if car returned in worse condition |
| Record penalty | −1 for late return, −2 for causing damage |
| Record bonus | +2 if car returned in better condition than rented |
| New customer record | Starts at **10** |
| Soft delete | Cars/users are marked `Deleted` in DB, not physically removed |

---

## 🔗 API Endpoints

| Method | Endpoint               | Description              |
|--------|------------------------|--------------------------|
| POST   | `/api/auth/login`      | Login (Customer/Manager) |
| POST   | `/api/auth/register`   | Register new customer    |
| GET    | `/api/cars`            | List all cars            |
| POST   | `/api/cars`            | Add a car (Manager only) |
| DELETE | `/api/cars/:id`        | Delete a car (Manager)   |
| POST   | `/api/cars/rent`       | Rent a car               |
| POST   | `/api/cars/return`     | Return a car             |
| GET    | `/api/users/:id`       | Get user profile         |
| POST   | `/api/users/pay-dues`  | Pay outstanding fine     |

---

## 🚀 Getting Started

### Prerequisites
- [Node.js](https://nodejs.org/) (v18 or above)
- npm

### 1. Install dependencies

```bash
# Backend
cd backend
npm install

# Frontend
cd ../frontend
npm install
```

### 2. Run the backend server

```bash
cd backend
node server.js
```

The backend will start on **http://localhost:3000**

### 3. Run the frontend dev server

```bash
cd frontend
npm run dev
```

The frontend will start on **http://localhost:5173**

### 4. Open in browser

Navigate to **http://localhost:5173** to use the application.

> **Default Manager credentials:** Check your `database.sqlite` or seed the manager user manually.

---

## 📌 Notes

- The SQLite database file (`backend/database.sqlite`) is auto-created on first run.
- All tables are initialized automatically via `database.js`.
- Passwords are stored in plain text (suitable for academic/demo purposes only — hash passwords for production use).
