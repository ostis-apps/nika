from abc import ABC, abstractmethod

from smtp.base.models import Mail


class SMTPService(ABC):

    @property
    @abstractmethod
    def password(self) -> str:
        pass

    @property
    @abstractmethod
    def host(self) -> str:
        pass

    @property
    @abstractmethod
    def port(self) -> str:
        pass

    @abstractmethod
    def send_mail(self, mail: Mail) -> bool:
        pass
