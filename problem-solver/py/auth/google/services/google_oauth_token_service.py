import requests

from auth.base.models import OauthClient
from auth.base.services import FernetCryptoService, OauthTokenService
from secrets_env import CRYPTO_KEY


TOKEN_URL = "https://oauth2.googleapis.com/token"


class GoogleOauthTokenService(OauthTokenService):
    def __init__(self, client: OauthClient):
        super().__init__(client, TOKEN_URL)
        self._crypto_service = FernetCryptoService(CRYPTO_KEY)

    @property
    def crypto_service(self):
        return self._crypto_service

    def get_tokens(self, code) -> dict[str, str]:
        response = requests.post(
            url=self.token_url,
            data={
                "client_id": self.client.id,
                "client_secret": self.client.secret,
                "code": code,
                "grant_type": "authorization_code",
                "redirect_uri": "http://localhost:3033/auth/google/callback",
            },
        )
        response.raise_for_status()
        res = response.json()
        crypto_acs_token = self.crypto_service.encrypt_token(
            res["access_token"],
            )
        crypto_ref_token = self.crypto_service.encrypt_token(
            res["refresh_token"],
            )
        return {
            "access_token": crypto_acs_token,
            "refresh_token": crypto_ref_token,
        }

    def is_token_valid(self, crypto_token: str):
        token = self.crypto_service.decrypt_token(crypto_token)
        response = requests.get(
            "https://www.googleapis.com/oauth2/v3/userinfo",
            headers={"Authorization": f"Bearer {token}"},
        )

        if response.status_code == 200:
            return True
        return False

    def get_new_token(
        self,
        crypto_refresh_token: str,
    ) -> str:
        refresh_token = self.crypto_service.decrypt_token(
            crypto_refresh_token,
            )
        payload = {
            "client_id": self.client.id,
            "client_secret": self.client.secret,
            "refresh_token": refresh_token,
            "grant_type": "refresh_token",
        }

        response = requests.post(
            self.token_url,
            data=payload,
            headers={
                "Content-Type": "application/x-www-form-urlencoded",
            },
            timeout=30,
        )

        if response.status_code == 200:
            token_data = response.json()
            crypto_acs_token = self.crypto_service.encrypt_token(
                token_data["access_token"],
            )
            return crypto_acs_token
