import React from 'react';

interface AuthMethodButtonProps {
  icon: React.ReactNode;
  text: string;
  onClick?: () => void;
  isLoading?: boolean;
  disabled?: boolean;
  className?: string;
  type?: 'default' | 'google' | 'email' | 'github';
}

const AuthMethodButton: React.FC<AuthMethodButtonProps> = ({
  icon,
  text,
  onClick,
  isLoading = false,
  disabled = false,
  className = '',
  type = 'default',
}) => {
  return (
    <button
      onClick={onClick}
      disabled={disabled || isLoading}
      className={`auth-page-button ${type}-button ${className}`}
    >
      <div className="auth-icon">
        {isLoading ? (
          <div className="spinner" />
        ) : (
          icon
        )}
      </div>
      <span>{isLoading ? 'Загрузка...' : text}</span>
    </button>
  );
};

export default AuthMethodButton;