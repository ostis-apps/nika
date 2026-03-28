import React from 'react';
import { PersonIcon } from './icons';
import { useHistory } from 'react-router-dom';
import { routes } from '@constants';

interface AuthButtonProps {
  size?: number;
  className?: string;
}

const AuthButton: React.FC<AuthButtonProps> = ({ size = 40, className = '' }) => {
  const history = useHistory();

  const handleClick = (e: React.MouseEvent) => {
    e.preventDefault();
    // Перенаправляем на страницу авторизации
    history.push(routes.AUTH);
  };

  return (
    <div className={`auth-button-container ${className}`}>
      <button
        onClick={handleClick}
        className="auth-button"
        style={{
          width: `${size}px`,
          height: `${size}px`,
          borderRadius: '50%',
          backgroundColor: '#287075ff',
          border: 'none',
          cursor: 'pointer',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          padding: 0,
          transition: 'all 0.2s ease',
        }}
        title="Войти"
        onMouseEnter={(e) => {
          e.currentTarget.style.opacity = '0.9';
          e.currentTarget.style.transform = 'scale(1.05)';
        }}
        onMouseLeave={(e) => {
          e.currentTarget.style.opacity = '1';
          e.currentTarget.style.transform = 'scale(1)';
        }}
      >
        <PersonIcon style={{ color: 'white' }} />
      </button>
    </div>
  );
};

export default AuthButton;