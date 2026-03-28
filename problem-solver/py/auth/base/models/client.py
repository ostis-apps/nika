from pydantic import BaseModel


class OauthClient(BaseModel):
    secret: str
    id: str
