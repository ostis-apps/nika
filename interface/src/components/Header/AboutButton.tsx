import React from 'react';
import { NavLink } from 'react-router-dom';
import { routes } from '@constants';
import { AboutIcon } from './icons';

interface AboutButtonProps {
  size?: number;
  className?: string;
}

const AboutButton: React.FC<AboutButtonProps> = ({ size = 40, className = '' }) => {
  return (
    <NavLink 
      to={routes.ABOUT}
      className={`about-button ${className}`}
      activeClassName="active"
      style={{
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        textDecoration: 'none'
      }}
    >
      <button
        className="about-button-inner"
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
        title="О нас"
        onMouseEnter={(e) => {
          e.currentTarget.style.backgroundColor = '#e0e0e0';
          e.currentTarget.style.transform = 'scale(1.1)';
        }}
        onMouseLeave={(e) => {
          e.currentTarget.style.backgroundColor = '#f0f0f0';
          e.currentTarget.style.transform = 'scale(1)';
        }}
      >
        <AboutIcon style={{ color: '#287075ff' }} />
      </button>
    </NavLink>
  );
};

export default AboutButton;

