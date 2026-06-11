import React, { useState } from 'react';
import { motion } from 'framer-motion';
import axios from 'axios';

export default function Login({ onLogin }) {
  const [role, setRole] = useState('Customer');
  const [name, setName] = useState('');
  const [password, setPassword] = useState('');
  const [mobile, setMobile] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [isRegistering, setIsRegistering] = useState(false);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');
    try {
      if (isRegistering) {
        const res = await axios.post('http://localhost:3000/api/auth/register', { name, password, mobile });
        onLogin(res.data.user);
      } else {
        const res = await axios.post('http://localhost:3000/api/auth/login', { name, password, role });
        onLogin(res.data.user);
      }
    } catch (err) {
      setError(err.response?.data?.error || (isRegistering ? 'Registration failed' : 'Login failed'));
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="flex-center">
      <motion.div 
        className="glass-panel animate-fade-in" 
        style={{ width: '100%', maxWidth: '400px' }}
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
      >
        <h2 style={{ textAlign: 'center', marginBottom: '24px' }}>
          {isRegistering ? 'Create Account' : 'Welcome Back'}
        </h2>
        
        {error && (
          <div style={{ background: 'rgba(239, 68, 68, 0.2)', color: '#fca5a5', padding: '12px', borderRadius: '8px', marginBottom: '16px', fontSize: '0.875rem' }}>
            {error}
          </div>
        )}

        <form onSubmit={handleSubmit}>
          {!isRegistering && (
            <div className="input-group">
              <label>Login As</label>
              <select className="input-field" value={role} onChange={(e) => setRole(e.target.value)}>
                <option value="Customer">Customer</option>
                <option value="Employee">Employee</option>
                <option value="Manager">Manager</option>
              </select>
            </div>
          )}
          
          <div className="input-group">
            <label>Username</label>
            <input 
              type="text" 
              className="input-field" 
              placeholder="Enter your username"
              value={name}
              onChange={(e) => setName(e.target.value)}
              required 
            />
          </div>

          <div className="input-group">
            <label>Password</label>
            <input 
              type="password" 
              className="input-field" 
              placeholder="Enter your password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required 
            />
          </div>

          {isRegistering && (
            <div className="input-group">
              <label>Mobile Number</label>
              <input 
                type="text" 
                className="input-field" 
                placeholder="Enter your mobile number"
                value={mobile}
                onChange={(e) => setMobile(e.target.value)}
                required 
              />
            </div>
          )}

          <button type="submit" className="btn btn-primary" style={{ width: '100%', marginTop: '8px' }} disabled={loading}>
            {loading ? 'Please wait...' : (isRegistering ? 'Register' : 'Login')}
          </button>
        </form>

        <div style={{ textAlign: 'center', marginTop: '16px' }}>
          <p style={{ fontSize: '0.875rem' }}>
            {isRegistering ? 'Already have an account? ' : "Don't have an account? "}
            <button 
              type="button" 
              onClick={() => { setIsRegistering(!isRegistering); setError(''); }}
              style={{ background: 'none', border: 'none', color: 'var(--primary)', cursor: 'pointer', fontWeight: 'bold' }}
            >
              {isRegistering ? 'Login here' : 'Register as new Customer'}
            </button>
          </p>
        </div>
      </motion.div>
    </div>
  );
}
