from datetime import UTC, datetime
from typing import Any

import requests

from cal.base.models import Date, Event, EventWithDate
from cal.base.services import EventService


class GoogleEventService(EventService):
    """Сервис для работы с событиями Google Calendar API"""

    BASE_URL = "https://www.googleapis.com/calendar/v3"

    def __init__(self, logger):
        super().__init__()
        self.calendar_id = "primary"
        self.logger = logger

    @property
    def url(self) -> str:
        """URL для работы с событиями календаря"""
        return f"{self.BASE_URL}/calendars/{self.calendar_id}/events"

    def _get_event_url(self, event_id: str) -> str:
        """URL для конкретного события"""
        return f"{self.url}/{event_id}"

    def _prepare_event_data(self, event: EventWithDate) -> dict[str, Any]:
        """
        Подготовка данных события для отправки в Google Calendar API

        Args:
            event: Объект события с датами

        Returns:
            Словарь с данными в формате Google Calendar API
        """
        event_data = {
            "summary": event.summary,
            "start": {
                "dateTime": event.start_date.iso,
                "timeZone": event.start_date.time_zone,
            },
            "end": {
                "dateTime": event.end_date.iso,
                "timeZone": event.start_date.time_zone,
            },
        }

        # Добавляем ID, если он есть
        if event.id:
            event_data["id"] = event.id

        return event_data

    def _parse_google_event(
        self,
        google_event: dict[str, Any],
    ) -> EventWithDate | None:
        """
        Парсинг события из формата Google Calendar в EventWithDate

        Args:
            google_event: Событие из ответа Google Calendar API

        Returns:
            EventWithDate или None в случае ошибки
        """
        try:
            # Извлекаем даты из ответа Google
            start_data = google_event.get("start", {})
            end_data = google_event.get("end", {})

            # Используем dateTime для событий с временем, date для событий на весь день
            start_iso = start_data.get("dateTime") or start_data.get(
                "date",
            )
            end_iso = end_data.get("dateTime") or end_data.get("date")

            if not start_iso:
                self.logger.error("Event has no start date")
                return None

            # Создаем объект EventWithDate
            return EventWithDate(
                id=google_event.get("id"),
                summary=google_event.get("summary", ""),
                start_date=Date(iso=start_iso),
                end_date=Date(iso=end_iso) if end_iso else None,
            )
        except Exception as e:
            self.logger.error(f"Error parsing Google event: {e}")
            return None

    def _handle_request_error(
        self,
        error: Exception,
        operation: str,
    ) -> None:
        """Обработка ошибок запросов"""
        if isinstance(error, requests.exceptions.ConnectionError):
            self.logger.error(f"{operation} failed: Connection error")
        else:
            self.logger.error(f"{operation} failed: {error}")

    def add_event(self, event) -> bool:
        """
        Добавление нового события в календарь

        Args:
            event: Объект события

        Returns:
            True если успешно, False в противном случае
        """
        # Проверяем, что событие имеет даты
        if not isinstance(event, EventWithDate):
            self.logger.error("Event must be of type EventWithDate")
            return False

        try:
            # Подготавливаем данные для отправки
            event_data = self._prepare_event_data(event)

            # Отправляем запрос
            response = requests.post(
                self.url,
                headers=self._base_headers,
                json=event_data,
                timeout=10,
            )

            # Проверяем ответ
            if response.status_code in (200, 201):
                created_event = response.json()
                event.id = created_event.get("id")
                self.logger.info(f"Event created successfully: {event.id}")
                return True

            # Логируем ошибку
            self.logger.error(
                f"Failed to create event: {response.status_code} - {response.text}",
            )
            return False

        except requests.exceptions.RequestException as e:
            self._handle_request_error(e, "Event creation")
            return False

    def delete_event(self, event) -> bool:
        """
        Удаление события из календаря

        Args:
            event: Объект события

        Returns:
            True если успешно, False в противном случае
        """
        if not event.id:
            self.logger.error("Cannot delete event without ID")
            return False

        try:
            response = requests.delete(
                self._get_event_url(event.id),
                headers=self._base_headers,
                timeout=10,
            )

            if response.status_code == 204:
                self.logger.info(f"Event deleted successfully: {event.id}")
                return True

            self.logger.error(
                f"Failed to delete event: {response.status_code} - {response.text}",
            )
            return False

        except requests.exceptions.RequestException as e:
            self._handle_request_error(e, "Event deletion")
            return False

    def search_event(self, event: Event) -> EventWithDate | None:
        """
        Поиск события в календаре

        Args:
            event: Объект события для поиска

        Returns:
            Найденное событие или None
        """
        # Если есть ID, ищем по ID
        if event.id:
            return self._get_event_by_id(event.id)

        # Иначе ищем по названию
        return self._search_event_by_summary(event.summary)

    def _get_event_by_id(self, event_id: str) -> EventWithDate | None:
        """Получение события по ID"""
        try:
            response = requests.get(
                self._get_event_url(event_id),
                headers=self._base_headers,
                timeout=10,
            )

            if response.status_code == 200:
                return self._parse_google_event(response.json())

            self.logger.error(
                f"Failed to get event by ID: {response.status_code} - {response.text}",
            )
            return None

        except requests.exceptions.RequestException as e:
            self._handle_request_error(e, "Get event by ID")
            return None

    def _search_event_by_summary(
        self,
        summary: str,
    ) -> EventWithDate | None:
        """Поиск события по названию"""
        time_min = datetime.now(UTC).replace(tzinfo=None).isoformat() + "Z"

        params = {
            "q": summary,
            "maxResults": "1",
            "timeMin": time_min,
            "singleEvents": "true",
            "orderBy": "startTime",
        }

        try:
            response = requests.get(
                self.url,
                headers=self._base_headers,
                params=params,
            )

            if response.status_code != 200:
                self.logger.error(
                    f"Search failed: {response.status_code} - {response.text}",
                )
                return None

            items = response.json().get("items", [])

            # Ищем точное совпадение по summary
            for item in items:
                if item.get("summary") == summary:
                    return self._parse_google_event(item)

            self.logger.info(f"No events found with summary: {summary}")
            return None

        except requests.exceptions.RequestException as e:
            self._handle_request_error(e, "Event search")
            return None

    def update_event(
        self,
        old_event: EventWithDate,
        new_event: EventWithDate,
    ) -> EventWithDate | None:
        """
        Обновление существующего события

        Args:
            old_event: Старое событие (должно содержать ID)
            new_event: Новые данные события

        Returns:
            Обновленное событие или None
        """
        if not old_event.id:
            self.logger.error("Cannot update event without ID")
            return None

        # Проверяем, что новое событие имеет даты
        if not isinstance(new_event, EventWithDate):
            self.logger.error("New event must be of type EventWithDate")
            return None

        try:
            # Подготавливаем данные для обновления
            update_data = self._prepare_event_data(new_event)

            # Отправляем PATCH запрос
            response = requests.patch(
                self._get_event_url(old_event.id),
                headers=self._payload_headers,
                json=update_data,
                timeout=10,
            )

            if response.status_code == 200:
                updated_event = response.json()
                self.logger.info(
                    f"Event updated successfully: {old_event.id}",
                )
                return self._parse_google_event(updated_event)

            self.logger.error(
                f"Failed to update event: {response.status_code} - {response.text}",
            )
            return None

        except requests.exceptions.RequestException as e:
            self._handle_request_error(e, "Event update")
            return None
