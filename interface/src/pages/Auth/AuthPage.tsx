import React from 'react';
import { Link } from 'react-router-dom';
import { routes } from '@constants';
import GoogleAuthButton from '@components/Auth/GoogleAuthButton';
import './AuthPage.css';

export const AuthPage = () => {
  return (
    <div className="auth-page-wrapper">
      <div className="auth-center-container">
        {/* Логотип на странице (опционально, можно убрать) */}
        <div className="auth-logo-container">
          <h1 className="auth-logo">NIKA</h1>
        </div>

        {/* Карточка с формой */}
        <div className="auth-card">
          <div className="auth-header">
            <h2 className="auth-title">Вход в систему</h2>
            <p className="auth-subtitle">Выберите способ авторизации</p>
          </div>

          <div className="auth-methods">
            {/* Google авторизация */}
            <div className="auth-method">
              <GoogleAuthButton />
            </div>

          </div>

          {/* Кнопка возврата */}
          <div className="auth-footer">
            <Link to={routes.MAIN} className="auth-back-link">
              <span className="back-icon">←</span>
              <span>Вернуться на главную</span>
            </Link>
          </div>
        </div>
      </div>
    </div>
  );
};