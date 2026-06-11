const fs = require('fs');
const csv = require('csv-parser');
const db = require('./database');
const path = require('path');

const basePath = path.resolve(__dirname, '..');

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

async function migrateUsers(csvFile, role) {
    return new Promise((resolve, reject) => {
        const users = [];
        fs.createReadStream(path.join(basePath, csvFile))
            .pipe(csv())
            .on('data', (data) => users.push(data))
            .on('end', async () => {
                for (let row of users) {
                    if(!row.Name) continue;
                    await runQuery(
                        `INSERT INTO users (name, password, role, fine_due, record, mobile, status) VALUES (?, ?, ?, ?, ?, ?, ?)`,
                        [
                            row.Name, 
                            row.Password || row.password || '', 
                            role, 
                            row.Fine_Due || 0, 
                            row.Record || 10, 
                            row.Mobile || '', 
                            row.Status || 'Existing'
                        ]
                    );
                }
                resolve();
            });
    });
}

async function migrateCars() {
    return new Promise((resolve, reject) => {
        const cars = [];
        fs.createReadStream(path.join(basePath, 'cars.csv'))
            .pipe(csv())
            .on('data', (data) => cars.push(data))
            .on('end', async () => {
                for (let row of cars) {
                    if(!row.Model) continue;
                    let rented_by_id = null;
                    if (row.Rented_By && row.Rented_By.trim() !== '') {
                        const user = await getQuery(`SELECT id FROM users WHERE name = ?`, [row.Rented_By.trim()]);
                        if (user) rented_by_id = user.id;
                    }
                    await runQuery(
                        `INSERT INTO cars (model, condition, due_date, rental_price, status, rented_by) VALUES (?, ?, ?, ?, ?, ?)`,
                        [
                            row.Model,
                            row.Condition || 'Good',
                            row.Due_date === 'NA' ? null : row.Due_date,
                            row.Rental_Price,
                            row.Status === 'Deleted' ? 'Deleted' : (row.Rented_By ? 'In_Use' : 'Available'),
                            rented_by_id
                        ]
                    );
                }
                resolve();
            });
    });
}

async function migrateAll() {
    console.log('Migrating Managers...');
    await migrateUsers('Manager.csv', 'Manager');
    console.log('Migrating Customers...');
    await migrateUsers('Customer.csv', 'Customer');
    console.log('Migrating Employees...');
    await migrateUsers('Employee.csv', 'Employee');
    console.log('Migrating Cars...');
    await migrateCars();
    console.log('Migration Complete!');
    db.close();
}

migrateAll().catch(console.error);
