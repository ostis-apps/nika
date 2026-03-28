from pydantic import BaseModel, EmailStr


class User(BaseModel):
    name: str
    email: EmailStr

    def __str__(self):
        return f"User<name={self.name}, email={self.email}>"
