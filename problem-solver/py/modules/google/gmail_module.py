from sc_kpm import ScModule

from smtp.google.agents import SendGoogleMailAgent


class GmailModule(ScModule):
    def __init__(self):
        super().__init__(
            SendGoogleMailAgent(),
        )
