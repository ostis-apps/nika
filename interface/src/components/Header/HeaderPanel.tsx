import { routes } from '@constants';
import * as React from 'react';
import { NavLink } from 'react-router-dom';

export const HeaderPanel = () => {
    return (
        <div className="header">
            <h1 className="header-logo-text">Ragneda</h1>
            <div className="nav-container">
                <ul className="nav">
                    <li>
                        <NavLink exact to={routes.HOME}>Главная</NavLink>
                    </li>
                    <li>
                        <NavLink to={routes.ABOUT}>Карта</NavLink>
                    </li>
                </ul>
            </div>
        </div>
    );
}
