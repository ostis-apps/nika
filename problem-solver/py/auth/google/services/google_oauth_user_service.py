import requests

from auth.base.models import User
from auth.base.services import FernetCryptoService, OauthUserService
from secrets_env import CRYPTO_KEY


USER_URL = "https://www.googleapis.com/oauth2/v2/userinfo"


class GoogleOauthUserService(OauthUserService):
    def __init__(self):
        super().__init__(USER_URL)
        self._crypto_service = FernetCryptoService(CRYPTO_KEY)

    @property
    def crypto_service(self):
        return self._crypto_service

    def get_user(self, crypto_token: str) -> User:
        token = self.crypto_service.decrypt_token(crypto_token)
        headers = {
            "Authorization": f"Bearer {token}",
            "Accept": "application/json",
        }
        response = requests.get(
            self.user_url,
            headers=headers,
            timeout=30,
        )
        response.raise_for_status()

        user_data = response.json()

        return User(
            name=user_data["name"],
            email=user_data["email"],
        )
