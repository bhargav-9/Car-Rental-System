import React from 'react';
import { LogOut, Car, User } from 'lucide-react';

export default function Navbar({ user, onLogout }) {
  return (
    <nav className="navbar">
      <div className="nav-brand">
        <Car style={{ display: 'inline', marginRight: '8px', verticalAlign: 'text-bottom' }} />
        Car Rental Pro
      </div>
      <div className="nav-links">
        <div className="badge badge-available" style={{ marginRight: '16px' }}>
          <User size={14} style={{ display: 'inline', marginRight: '4px', verticalAlign: 'text-bottom' }} />
          {user.name} ({user.role})
        </div>
        <button onClick={onLogout} className="btn btn-danger" style={{ padding: '8px 16px', fontSize: '0.875rem' }}>
          <LogOut size={16} /> Logout
        </button>
      </div>
    </nav>
  );
}
