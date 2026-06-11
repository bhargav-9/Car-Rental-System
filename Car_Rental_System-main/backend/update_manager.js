const db = require('./database');

db.serialize(() => {
    db.run("DELETE FROM users WHERE role = 'Manager'", (err) => {
        if (err) console.error(err);
        console.log('Old managers deleted');
    });

    db.run("INSERT INTO users (name, password, role, mobile, fine_due, record, status) VALUES ('Bhargav', '1234b', 'Manager', '9999999999', 0, 10, 'Existing')", (err) => {
        if (err) console.error(err);
        console.log('New manager Bhargav added');
    });
});

setTimeout(() => process.exit(0), 1000);
