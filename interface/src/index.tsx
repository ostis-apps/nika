import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { BrowserRouter } from 'react-router-dom';
import { createGlobalStyle } from 'styled-components';

import { store } from '@store';

import { App } from './App';

const GlobalStyle = createGlobalStyle`
  body {
    margin: 0;
    display: flex;
    font-family: 'Roboto', sans-serif;
    /* For firefox full height */
    height: 100%;
  }
  #content {
    flex-grow: 1;
    display: flex;
  }
  * {
    box-sizing: border-box;
  }
`;

ReactDOM.render(
    <Provider store={store}>
        <BrowserRouter>
            <GlobalStyle />
            <App />
        </BrowserRouter>
    </Provider>,
    document.getElementById('content'),
);
