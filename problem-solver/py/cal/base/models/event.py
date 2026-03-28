from datetime import datetime, timedelta

from pydantic import BaseModel, model_validator

from cal.base.models import Date


class Event(BaseModel):
    id: str | None = None
    summary: str


class EventWithDate(Event):
    start_date: Date
    end_date: Date | None = None

    @model_validator(mode="after")
    def set_end(self) -> "EventWithDate":
        if self.end_date is None:
            end_time = datetime.fromisoformat(
                self.start_date.iso,
            ) + timedelta(hours=1)

            iso_end_time = datetime.isoformat(end_time)
            self.end_date = Date(iso=iso_end_time)
        return self
