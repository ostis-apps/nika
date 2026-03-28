from sc_kpm import ScModule

from cal.google.agents import (
    AddGoogleEventAgent,
    DeleteGoogleEventAgent,
    UpdateGoogleEventAgent,
)


class GoogleCalendarModule(ScModule):
    def __init__(self):
        super().__init__(
            AddGoogleEventAgent(),
            DeleteGoogleEventAgent(),
            UpdateGoogleEventAgent(),
        )
