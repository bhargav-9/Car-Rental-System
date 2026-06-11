const sqlite3 = require('sqlite3').verbose();
const path = require('path');

const dbPath = path.resolve(__dirname, 'database.sqlite');
const db = new sqlite3.Database(dbPath, (err) => {
    if (err) {
        console.error('Error connecting to database:', err.message);
    } else {
        console.log('Connected to the SQLite database.');
    }
});

// Initialize tables
db.serialize(() => {
    db.run(`CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        password TEXT NOT NULL,
        role TEXT NOT NULL,
        fine_due INTEGER DEFAULT 0,
        record INTEGER DEFAULT 10,
        mobile TEXT,
        status TEXT DEFAULT 'Existing'
    )`);

    db.run(`CREATE TABLE IF NOT EXISTS cars (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        model TEXT NOT NULL,
        condition TEXT DEFAULT 'Good',
        due_date TEXT,
        rental_price INTEGER NOT NULL,
        status TEXT DEFAULT 'Available',
        rented_by INTEGER,
        FOREIGN KEY (rented_by) REFERENCES users (id)
    )`);

    db.run(`CREATE TABLE IF NOT EXISTS rentals (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        car_id INTEGER,
        user_id INTEGER,
        start_date TEXT,
        end_date TEXT,
        total_price INTEGER,
        status TEXT,
        FOREIGN KEY (car_id) REFERENCES cars (id),
        FOREIGN KEY (user_id) REFERENCES users (id)
    )`);
});

module.exports = db;
