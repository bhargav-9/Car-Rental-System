import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { motion } from 'framer-motion';

export default function Dashboard({ user }) {
  const [currentUser, setCurrentUser] = useState(user);
  const [cars, setCars] = useState([]);
  const [loading, setLoading] = useState(true);
  const [rentDays, setRentDays] = useState(1);
  const [returnCondition, setReturnCondition] = useState('Good');
  const [lateDays, setLateDays] = useState(0);
  const [payAmount, setPayAmount] = useState('');

  const fetchUser = async () => {
    try {
      const res = await axios.get(`http://localhost:3000/api/users/${user.id}`);
      setCurrentUser(res.data);
    } catch (err) {
      console.error(err);
    }
  };

  const fetchCars = async () => {
    try {
      const res = await axios.get('http://localhost:3000/api/cars');
      setCars(res.data);
    } catch (err) {
      console.error(err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchCars();
    fetchUser();
  }, []);

  const handleRent = async (carId) => {
    try {
      await axios.post('http://localhost:3000/api/cars/rent', {
        car_id: carId,
        user_id: user.id,
        days: rentDays
      });
      alert('Car rented successfully!');
      fetchCars();
    } catch (err) {
      alert(err.response?.data?.error || 'Failed to rent car');
    }
  };

  const handleReturn = async (carId) => {
    try {
      const res = await axios.post('http://localhost:3000/api/cars/return', {
        car_id: carId,
        user_id: user.id,
        condition_returned: returnCondition,
        late_days: lateDays
      });
      alert(`Car returned successfully! Fine added: ${res.data.fine_added}, Record change: ${res.data.record_change}`);
      fetchCars();
    } catch (err) {
      alert(err.response?.data?.error || 'Failed to return car');
    }
  };

  const handlePayDues = async (e) => {
    e.preventDefault();
    if (Number(payAmount) <= 0) return alert('Enter a valid amount');
    try {
      const res = await axios.post('http://localhost:3000/api/users/pay-dues', {
        user_id: user.id,
        amount: Number(payAmount)
      });
      alert('Dues paid successfully!');
      setCurrentUser(res.data.user);
      setPayAmount('');
    } catch (err) {
      alert(err.response?.data?.error || 'Failed to pay dues');
    }
  };

  if (loading) return <div className="flex-center">Loading...</div>;

  return (
    <div className="container animate-fade-in">
      
      <motion.div className="glass-panel" style={{ marginBottom: '40px', background: 'rgba(30, 41, 59, 0.9)' }}>
        <h2 style={{ margin: 0, marginBottom: '16px' }}>My Profile</h2>
        <div style={{ display: 'flex', gap: '32px', flexWrap: 'wrap' }}>
          <div>
            <p style={{ margin: 0 }}><strong>Record Rating:</strong> {currentUser.record}</p>
            <p style={{ margin: 0, color: currentUser.fine_due > 0 ? '#ef4444' : '#10b981' }}>
              <strong>Fine Due:</strong> Rs. {currentUser.fine_due}
            </p>
          </div>
          {currentUser.fine_due > 0 && (
            <form onSubmit={handlePayDues} style={{ display: 'flex', gap: '8px', alignItems: 'flex-end' }}>
              <div className="input-group" style={{ marginBottom: 0 }}>
                <label>Amount to Pay (Rs.)</label>
                <input 
                  type="number" 
                  max={currentUser.fine_due} 
                  value={payAmount} 
                  onChange={(e) => setPayAmount(e.target.value)} 
                  className="input-field" 
                  style={{ padding: '8px', width: '150px' }} 
                  required 
                />
              </div>
              <button type="submit" className="btn btn-primary" style={{ padding: '8px 16px' }}>Pay Dues</button>
            </form>
          )}
        </div>
      </motion.div>

      <h2>Available Cars to Rent</h2>
      <div className="grid-cards" style={{ marginBottom: '40px' }}>
        {cars.filter(c => c.status === 'Available').map(car => (
          <motion.div key={car.id} className="glass-panel" whileHover={{ y: -5 }}>
            <h3>{car.model}</h3>
            <p>Condition: {car.condition}</p>
            <p>Price: Rs. {car.rental_price} / day</p>
            <div className="input-group" style={{ marginTop: '16px' }}>
              <label>Days to Rent:</label>
              <input type="number" min="1" max="15" value={rentDays} onChange={(e) => setRentDays(Number(e.target.value))} className="input-field" style={{ padding: '8px' }} />
            </div>
            <button onClick={() => handleRent(car.id)} className="btn btn-primary" style={{ width: '100%', marginTop: '8px' }}>Rent Now</button>
          </motion.div>
        ))}
        {cars.filter(c => c.status === 'Available').length === 0 && <p>No cars available at the moment.</p>}
      </div>

      <h2>Your Rented Cars</h2>
      <div className="grid-cards">
        {cars.filter(c => c.rented_by === user.id).map(car => (
          <motion.div key={car.id} className="glass-panel" style={{ borderLeft: '4px solid var(--primary)' }}>
            <h3>{car.model}</h3>
            <p>Due Date: {car.due_date}</p>
            <div className="input-group" style={{ marginTop: '16px' }}>
              <label>Condition Returned:</label>
              <select value={returnCondition} onChange={(e) => setReturnCondition(e.target.value)} className="input-field" style={{ padding: '8px' }}>
                <option value="Good">Good</option>
                <option value="Damaged">Damaged</option>
              </select>
            </div>
            <div className="input-group">
              <label>Late by (Days):</label>
              <input type="number" min="0" value={lateDays} onChange={(e) => setLateDays(Number(e.target.value))} className="input-field" style={{ padding: '8px' }} />
            </div>
            <button onClick={() => handleReturn(car.id)} className="btn btn-primary" style={{ width: '100%', marginTop: '8px' }}>Return Car</button>
          </motion.div>
        ))}
        {cars.filter(c => c.rented_by === user.id).length === 0 && <p>You have no rented cars.</p>}
      </div>
    </div>
  );
}
