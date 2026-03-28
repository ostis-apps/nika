import AuthButton from './AuthButton';
import HomeButton from './HomeButton';
import AboutButton from './AboutButton';

export const HeaderPanel = () => {
  return (
    <div className="header">
      <h1 className="header-logo-text">NIKA</h1>
      
      <div className="nav-container">
        <ul className="nav">
          <li>
            <HomeButton />
          </li>
          <li>
            <AboutButton />
          </li>
          <li>
            <AuthButton />
          </li>
        </ul>
      </div>
    </div>
  );
};