import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { motion } from 'framer-motion';

export default function ManagerDashboard({ user }) {
  const [cars, setCars] = useState([]);
  const [loading, setLoading] = useState(true);
  const [newCarModel, setNewCarModel] = useState('');
  const [newCarPrice, setNewCarPrice] = useState('');

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
  }, []);

  const handleAddCar = async (e) => {
    e.preventDefault();
    try {
      await axios.post('http://localhost:3000/api/cars', { model: newCarModel, rental_price: Number(newCarPrice) });
      setNewCarModel('');
      setNewCarPrice('');
      fetchCars();
    } catch (err) {
      alert(err.response?.data?.error || 'Failed to add car');
    }
  };

  const handleDeleteCar = async (id) => {
    if (!window.confirm('Are you sure you want to delete this car?')) return;
    try {
      await axios.delete(`http://localhost:3000/api/cars/${id}`);
      fetchCars();
    } catch (err) {
      alert(err.response?.data?.error || 'Failed to delete car');
    }
  };

  if (loading) return <div className="flex-center">Loading...</div>;

  return (
    <div className="container animate-fade-in">
      <h2>Manager Control Panel</h2>
      
      <motion.div className="glass-panel" style={{ marginBottom: '40px', maxWidth: '600px' }}>
        <h3>Add New Car</h3>
        <form onSubmit={handleAddCar} style={{ display: 'flex', gap: '16px', alignItems: 'flex-end' }}>
          <div className="input-group" style={{ flex: 1, marginBottom: 0 }}>
            <label>Model Name</label>
            <input type="text" value={newCarModel} onChange={(e) => setNewCarModel(e.target.value)} className="input-field" required />
          </div>
          <div className="input-group" style={{ flex: 1, marginBottom: 0 }}>
            <label>Rental Price / Day (Rs.)</label>
            <input type="number" value={newCarPrice} onChange={(e) => setNewCarPrice(e.target.value)} className="input-field" required />
          </div>
          <button type="submit" className="btn btn-primary">Add Car</button>
        </form>
      </motion.div>

      <h2>All System Cars</h2>
      <div className="grid-cards">
        {cars.map(car => (
          <motion.div key={car.id} className="glass-panel" whileHover={{ y: -5 }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '8px' }}>
              <h3 style={{ margin: 0 }}>{car.model}</h3>
              <span className={`badge ${car.status === 'Available' ? 'badge-available' : 'badge-inuse'}`}>
                {car.status}
              </span>
            </div>
            <p style={{ margin: '4px 0' }}>Condition: {car.condition}</p>
            <p style={{ margin: '4px 0' }}>Price: Rs. {car.rental_price} / day</p>
            {car.status === 'In_Use' && (
              <p style={{ margin: '4px 0', fontSize: '0.875rem', color: 'var(--primary)' }}>
                Rented by: {car.rented_by_name} (Due: {car.due_date})
              </p>
            )}
            <button 
              onClick={() => handleDeleteCar(car.id)} 
              className="btn btn-danger" 
              style={{ width: '100%', marginTop: '16px', padding: '8px' }}
              disabled={car.status === 'In_Use'}
            >
              Delete Car
            </button>
          </motion.div>
        ))}
      </div>
    </div>
  );
}
