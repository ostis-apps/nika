import React from 'react';
import { NavLink } from 'react-router-dom';
import { routes } from '@constants';
import { HomeIcon } from './icons';


interface HomeButtonProps {
  size?: number;
  className?: string;
}

const HomeButton: React.FC<HomeButtonProps> = ({ size = 40, className = '' }) => {
  return (
    <NavLink 
      exact 
      to={routes.MAIN}
      className={`home-button ${className}`}
      activeClassName="active"
      style={{
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        textDecoration: 'none'
      }}
    >
      <button
        className="home-button-inner"
        style={{
          width: `${size}px`,
          height: `${size}px`,
          borderRadius: '50%',
          backgroundColor: '#f0f0f0',
          border: '2px solid #287075ff',
          cursor: 'pointer',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          padding: 0,
          transition: 'all 0.2s ease',
        }}
        title="Главная"
        onMouseEnter={(e) => {
          e.currentTarget.style.backgroundColor = '#e0e0e0';
          e.currentTarget.style.transform = 'scale(1.1)';
        }}
        onMouseLeave={(e) => {
          e.currentTarget.style.backgroundColor = '#f0f0f0';
          e.currentTarget.style.transform = 'scale(1)';
        }}
      >
        <HomeIcon style={{ color: '#287075ff' }} />
      </button>
    </NavLink>
  );
};

export default HomeButton;


