const express = require('express');
const cors = require('cors');
const db = require('./database');

const app = express();
app.use(cors());
app.use(express.json());

// Helper for queries
function runQuery(query, params = []) {
    return new Promise((resolve, reject) => {
        db.run(query, params, function(err) {
            if (err) reject(err);
            else resolve(this);
        });
    });
}
function getQuery(query, params = []) {
    return new Promise((resolve, reject) => {
        db.get(query, params, (err, row) => {
            if (err) reject(err);
            else resolve(row);
        });
    });
}
function allQuery(query, params = []) {
    return new Promise((resolve, reject) => {
        db.all(query, params, (err, rows) => {
            if (err) reject(err);
            else resolve(rows);
        });
    });
}

// -----------------------------------------
// AUTHENTICATION
// -----------------------------------------
app.post('/api/auth/login', async (req, res) => {
    const { name, password, role } = req.body;
    try {
        const user = await getQuery('SELECT * FROM users WHERE name = ? AND password = ? AND role = ?', [name, password, role]);
        if (user) {
            if (user.status === 'Deleted') {
                return res.status(403).json({ error: 'Account has been deleted or blocked.' });
            }
            res.json({ message: 'Login successful', user });
        } else {
            res.status(401).json({ error: 'Invalid credentials' });
        }
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.post('/api/auth/register', async (req, res) => {
    const { name, password, mobile } = req.body;
    try {
        const existing = await getQuery('SELECT * FROM users WHERE name = ? AND role = ?', [name, 'Customer']);
        if (existing) {
            return res.status(400).json({ error: 'Username already exists' });
        }
        const result = await runQuery(
            `INSERT INTO users (name, password, role, fine_due, record, mobile, status) VALUES (?, ?, ?, ?, ?, ?, ?)`,
            [name, password, 'Customer', 0, 10, mobile || '', 'Existing']
        );
        const newUser = await getQuery('SELECT * FROM users WHERE id = ?', [result.lastID]);
        res.status(201).json({ message: 'Registration successful', user: newUser });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

// -----------------------------------------
// CARS
// -----------------------------------------
app.get('/api/cars', async (req, res) => {
    try {
        const cars = await allQuery(`
            SELECT cars.*, users.name as rented_by_name 
            FROM cars 
            LEFT JOIN users ON cars.rented_by = users.id 
            WHERE cars.status != 'Deleted'
        `);
        res.json(cars);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.post('/api/cars', async (req, res) => {
    const { model, rental_price } = req.body;
    try {
        const result = await runQuery(
            `INSERT INTO cars (model, rental_price) VALUES (?, ?)`,
            [model, rental_price]
        );
        res.status(201).json({ id: result.lastID, model, rental_price, condition: 'Good', status: 'Available' });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.delete('/api/cars/:id', async (req, res) => {
    try {
        const car = await getQuery('SELECT * FROM cars WHERE id = ?', [req.params.id]);
        if (!car) return res.status(404).json({ error: 'Car not found' });
        if (car.status === 'In_Use') return res.status(403).json({ error: 'Cannot delete a car that is currently in use.' });
        
        await runQuery(`UPDATE cars SET status = 'Deleted' WHERE id = ?`, [req.params.id]);
        res.json({ message: 'Car deleted successfully' });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

// -----------------------------------------
// RENTALS
// -----------------------------------------
app.post('/api/cars/rent', async (req, res) => {
    const { car_id, user_id, days } = req.body;
    try {
        const user = await getQuery('SELECT * FROM users WHERE id = ?', [user_id]);
        if (user.fine_due > 50000) return res.status(403).json({ error: 'Fine due exceeds 50,000. Pay fine first.' });
        if (user.record <= 0) return res.status(403).json({ error: 'Record is too low to rent a car.' });

        const car = await getQuery('SELECT * FROM cars WHERE id = ?', [car_id]);
        if (!car || car.status !== 'Available') return res.status(400).json({ error: 'Car is not available.' });

        const startDate = new Date().toISOString().split('T')[0];
        const dueDate = new Date(Date.now() + days * 24 * 60 * 60 * 1000).toISOString().split('T')[0];

        await runQuery('UPDATE cars SET status = ?, rented_by = ?, due_date = ? WHERE id = ?', ['In_Use', user_id, dueDate, car_id]);
        
        await runQuery(
            `INSERT INTO rentals (car_id, user_id, start_date, end_date, total_price, status) VALUES (?, ?, ?, null, ?, 'Active')`,
            [car_id, user_id, startDate, car.rental_price * days]
        );

        res.json({ message: 'Car rented successfully', due_date: dueDate });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.post('/api/cars/return', async (req, res) => {
    const { car_id, user_id, condition_returned, late_days } = req.body;
    try {
        const car = await getQuery('SELECT * FROM cars WHERE id = ?', [car_id]);
        const user = await getQuery('SELECT * FROM users WHERE id = ?', [user_id]);
        
        if (!car || car.rented_by !== user_id) return res.status(400).json({ error: 'Invalid return request.' });

        let fine = 0;
        let record_change = 0;

        // Late fines
        if (late_days > 0) {
            fine += Math.floor(1.5 * car.rental_price * late_days);
            record_change -= 1;
        }

        // Damage fines
        let new_condition = car.condition;
        if (car.condition === 'Good' && condition_returned === 'Damaged') {
            fine += 5000;
            record_change -= 2;
            new_condition = 'Damaged';
        } else if (car.condition === 'Damaged' && condition_returned === 'Good') {
            record_change += 2;
            new_condition = 'Good';
        }

        const new_fine = user.fine_due + fine;
        const new_record = user.record + record_change;

        // Update user
        await runQuery('UPDATE users SET fine_due = ?, record = ? WHERE id = ?', [new_fine, new_record, user_id]);
        
        // Update car
        await runQuery('UPDATE cars SET status = ?, rented_by = NULL, due_date = NULL, condition = ? WHERE id = ?', ['Available', new_condition, car_id]);

        // Update rental
        await runQuery('UPDATE rentals SET end_date = ?, status = ? WHERE car_id = ? AND user_id = ? AND status = ?', 
            [new Date().toISOString().split('T')[0], 'Returned', car_id, user_id, 'Active']);

        res.json({ message: 'Car returned successfully', fine_added: fine, record_change });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

// -----------------------------------------
// USERS
// -----------------------------------------
app.get('/api/users/:id', async (req, res) => {
    try {
        const user = await getQuery('SELECT * FROM users WHERE id = ?', [req.params.id]);
        res.json(user);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.post('/api/users/pay-dues', async (req, res) => {
    const { user_id, amount } = req.body;
    try {
        const user = await getQuery('SELECT * FROM users WHERE id = ?', [user_id]);
        if (!user) return res.status(404).json({ error: 'User not found' });
        
        let new_fine = user.fine_due - amount;
        if (new_fine < 0) new_fine = 0;

        await runQuery('UPDATE users SET fine_due = ? WHERE id = ?', [new_fine, user_id]);
        
        const updatedUser = await getQuery('SELECT * FROM users WHERE id = ?', [user_id]);
        res.json({ message: 'Dues paid successfully', user: updatedUser });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
