import base64

from abc import ABC

from cryptography.fernet import Fernet

from secrets_env import CRYPTO_KEY


class CryptoService(ABC):
    ...


class FernetCryptoService(CryptoService):
    def __init__(self, key: str):
        """Инициализация с ключом"""
        self.key = base64.urlsafe_b64encode(
            key.encode()[:32].ljust(32, b"\0"),
        )
        self.fernet = Fernet(self.key)

    def encrypt_token(self, token_data: str) -> str:
        """Шифрование токена"""
        encrypted = self.fernet.encrypt(token_data.encode())
        return base64.urlsafe_b64encode(encrypted).decode()

    def decrypt_token(self, encrypted_token: str) -> str:
        """Дешифрование токена"""
        try:
            encrypted_bytes = base64.urlsafe_b64decode(
                encrypted_token.encode(),
            )
            decrypted = self.fernet.decrypt(encrypted_bytes)
            return decrypted.decode()
        except Exception as e:
            raise ValueError(f"Token decryption failed: {str(e)}")

    def get_key(self) -> str:
        """Получить ключ в виде строки"""
        return self.key.decode()


crypto_service = FernetCryptoService(CRYPTO_KEY)
