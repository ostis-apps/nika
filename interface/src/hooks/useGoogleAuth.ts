import { useState, useCallback, useEffect } from 'react';


export const useGoogleAuth = () => {
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const handleGoogleAuth = useCallback(() => {
    setIsLoading(true);
    setError(null);
    // use oauth2.0 technology
    // send request to google(get code query param)
    const clientId = process.env.GOOGLE_CLIENT_ID;
    const redirectUri = 'http://localhost:3033/auth/google/callback';
    const scopes = [
      'email', 
      'profile', 
      'https://www.googleapis.com/auth/calendar',
      'https://mail.google.com/',
    ];
    const responseType = 'code';
    
    const authUrl = `https://accounts.google.com/o/oauth2/v2/auth?` +
      `client_id=${clientId}` +
      `&redirect_uri=${encodeURIComponent(redirectUri)}` +
      `&response_type=${responseType}` +
      `&scope=${encodeURIComponent(scopes.join(' '))}` +
      `&access_type=offline` +
      `&prompt=consent`;

    window.location.href = authUrl;
  }, []);

  return {
    handleGoogleAuth,
    isLoading,
    error,
  };
};