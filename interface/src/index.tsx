import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { BrowserRouter } from 'react-router-dom';
import { createGlobalStyle } from 'styled-components';

import { store } from '@store';

import { App } from './App';
import { CookiesProvider } from 'react-cookie';

const GlobalStyle = createGlobalStyle`
@import url('https://fonts.googleapis.com/css2?family=Montserrat:wght@300;400;700;800&display=swap');
  body {
    margin: 0;
    display: flex;
    font-family: 'Montserrat', sans-serif;
    /* For firefox full height */
    height: 100%;
    width: 100%;
    background: #101514;
    padding: 0;
    overflow: hidden;
    -webkit-tap-highlight-color: transparent;
  }
  #content {
    flex-grow: 1;
    display: flex;
    background: black;
  }
  * {
    box-sizing: border-box;
  }

  @-webkit-keyframes zoomIn {
    from {
      opacity: 0;
      -webkit-transform: scale3d(0.3, 0.3, 0.3);
      transform: scale3d(0.3, 0.3, 0.3);
    }
  
    50% {
      opacity: 1;
    }
  }


@media screen and (max-width: 500px) {
  .container_menu {
      width: 100vw !important;
      border-radius: 0 !important;
  }
}

@media screen and (max-width: 370px) {
   .nav {
      width: 24px;
   }

   .nav .title {
      display: none;
   }
}

@keyframes wave {
  0% {
    transform: rotate(0);
  }
  100% {
    transform: rotate(360deg);
  }
}
`;

ReactDOM.render(
    <Provider store={store}>
        <BrowserRouter>
            <GlobalStyle />
            <CookiesProvider>
                <App />
            </CookiesProvider>
        </BrowserRouter>
    </Provider>,
    document.getElementById('content'),
);
