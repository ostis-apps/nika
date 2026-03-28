from abc import ABC, abstractmethod

from auth.base.services import CryptoService


class OauthService(ABC):
    @property
    @abstractmethod
    def crypto_service(self) -> CryptoService:
        pass
