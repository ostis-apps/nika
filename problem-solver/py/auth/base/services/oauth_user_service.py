from abc import ABC, abstractmethod

from auth.base.models import User
from auth.base.services import OauthService


class OauthUserService(OauthService, ABC):
    def __init__(self, user_url: str):
        self.user_url = user_url

    @abstractmethod
    def get_user(self, token: str) -> User:
        pass
