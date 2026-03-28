from sc_kpm import ScModule

from auth.google.agents import (
    CheckGoogleTokenAgent,
    CreateGoogleAuthorAgent,
    GoogleCreateTokensAgent,
)


class GoogleAuthModule(ScModule):
    def __init__(self):
        super().__init__(
            CheckGoogleTokenAgent(),
            CreateGoogleAuthorAgent(),
            GoogleCreateTokensAgent(),
        )
