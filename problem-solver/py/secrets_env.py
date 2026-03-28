import configparser

from pathlib import Path


project_root = Path(__file__).parent.parent.parent
nika_ini_path = project_root / 'nika.ini'
config = configparser.ConfigParser()
config.read(nika_ini_path, encoding='utf-8')

GOOGLE_CLIENT_ID = config.get('google', 'client_id')
GOOGLE_API_KEY = config.get('google', 'api_key')
GOOGLE_CLIENT_SECRET = config.get('google', 'client_secret')
GMAIL_PASS = config.get('google', 'app_password')
CRYPTO_KEY = config.get('crypto', 'key')
