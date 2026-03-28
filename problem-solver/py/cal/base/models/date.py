from pydantic import BaseModel


class Date(BaseModel):
    iso: str  # iso format
    time_zone: str = "Europe/Moscow"
