// GoogleCallback.tsx
import { useEffect, useState } from 'react';
import { useHistory, useLocation } from 'react-router-dom';
import { call_create_author_agent } from '@api/sc/agents/googleAuthAgent';
import { generateSessionId, setCookie, waitForAction } from '@utils';
import { ScAddr, ScEventSubscriptionParams, ScEventType } from 'ts-sc-client';
import { client } from '@api';


export const GoogleCallback = () => {
  const history = useHistory();
  const location = useLocation();
  const [error, setError] = useState<string | null>(null);
  
  const handleGoogleCode = async (code: string) => {
    try {
      const session = generateSessionId();
      setCookie('auth_session', session);
      console.log('Generate auth session:', session);

      const action_node = await call_create_author_agent(
        code, 
        session, 
        "action_create_google_author"
      );
      if (action_node) {
        await waitForAction(action_node)
      }

      history.push('/');
      
    } catch (error) {
      console.error('Get error with auth:', error);
      setError('Authentication failed');
      setTimeout(() => {
        history.push('/login');
      }, 3000);
    }
  };

  useEffect(() => {
    const urlParams = new URLSearchParams(location.search);
    const code = urlParams.get('code');
    
    if (code) {      
      handleGoogleCode(code);
      // Очищаем URL от параметров code
      const cleanUrl = window.location.pathname;
      window.history.replaceState({}, '', cleanUrl);
    } else {
      // Если нет кода, редиректим на главную
      history.replace('/');
    }
  }, [location.search]);

  if (error) {
    return (
      <div style={{
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        height: '100vh',
        flexDirection: 'column'
      }}>
        <div style={{ color: 'red', marginBottom: '20px' }}>{error}</div>
        <div>Redirecting to login page...</div>
      </div>
    );
  }

  return (
    <div style={{
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
      height: '100vh'
    }}>
      <div>Processing Google authentication...</div>
    </div>
  );
};

export default GoogleCallback;