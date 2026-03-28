from pydantic import BaseModel

from auth.base.models import User


class Mail(BaseModel):
    sender: User
    receiver: User
    body: str
    subject: str | None = None
