from abc import ABC, abstractmethod

from auth.base.models import OauthClient
from auth.base.services import (
    OauthService,
)


class OauthTokenService(OauthService, ABC):
    def __init__(self, client: OauthClient, token_url: str):
        self.client = client
        self.token_url = token_url

    @abstractmethod
    def get_tokens(self, code: str) -> dict[str, str]:
        pass

    @abstractmethod
    def is_token_valid(self, token: str) -> bool:
        pass

    @abstractmethod
    def get_new_token(self, refresh_token: str) -> str:
        pass
