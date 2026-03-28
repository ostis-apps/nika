import smtplib

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

from secrets_env import GMAIL_PASS
from smtp.base.models import Mail
from smtp.base.services import SMTPService


class GoogleSMTPService(SMTPService):

    @property
    def password(self) -> str | None:
        return GMAIL_PASS

    @property
    def host(self) -> str:
        return "smtp.gmail.com"

    @property
    def port(self) -> str:
        return 587

    def send_mail(self, mail: Mail) -> bool:
        # Создание сообщения
        smtp_mail = MIMEMultipart()
        smtp_mail["From"] = mail.sender.email
        smtp_mail["To"] = mail.receiver.email
        smtp_mail["Subject"] = mail.subject
        smtp_mail.attach(MIMEText(mail.body, "plain"))

        try:
            with smtplib.SMTP(self.host, self.port) as server:
                server.starttls()
                server.login(mail.sender.email, GMAIL_PASS)
                text = smtp_mail.as_string()
                server.sendmail(mail.sender.email, mail.receiver.email, text)
            return True
        except Exception:
            raise
