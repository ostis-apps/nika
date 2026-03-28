from abc import ABC, abstractmethod

from cal.base.models import Event, EventWithDate


class EventService(ABC):
    def __init__(self):
        super().__init__()
        self._token: str | None = None
        self._base_headers: dict | None = None
        # Заголовки для запросов с телом (POST, PATCH, PUT)
        self._payload_headers: dict | None = None

    @property
    def token(self) -> str:
        return self._token

    @token.setter
    def token(self, token: str):
        self._token = token
        self._base_headers = {
            "Authorization": f"Bearer {self._token}",
            "Accept": "application/json",
        }

        self._payload_headers = {
            **self._base_headers,
            "Content-Type": "application/json",
        }

    @property
    @abstractmethod
    def url(self) -> str:
        pass

    @abstractmethod
    def search_event(self, event: Event) -> EventWithDate | None:
        pass

    @abstractmethod
    def delete_event(self, event: Event) -> bool:
        pass

    @abstractmethod
    def update_event(
        self,
        old_event: Event,
        new_event: EventWithDate,
    ) -> EventWithDate | None:
        pass

    @abstractmethod
    def add_event(self, event: EventWithDate) -> bool:
        pass
