// LotVisitText - translated LotVisit texts.
#pragma once

namespace LotVisitText
{
    enum Id
    {
        kFilterTip,
        kTiredTitle,
        kTired,
        kDayTitle,
        kDay,
        kWorkTitle,
        kWork,
        kTiredWarnTitle,
        kTiredWarn,
        kDayWarnTitle,
        kDayWarn,
        kWorkWarnTitle,
        kWorkWarn,
        kShownOutTitle,
        kShownOut,
        kDoorOpenTitle,
        kDoorOpen,
        kHungryWarnTitle,
        kHungryWarn,
        kHungryTitle,
        kHungry,
        kGuestsGoneTitle,
        kGuestsGoneHome,
        kGuestsGoneNbhd,
        kHostsGoneTitle,
        kHostsGone,
        kSleepyWarnTitle,
        kSleepyWarn,
        kSleepyTitle,
        kSleepy,
        kVisitHint,
        kLeaveVisitTitle,
        kLeaveVisit,
    };
    // Text in the current game language (its code page), US English when unknown; a random variant unless nVariant is given.
    const char *Get(Id nId, int nVariant = -1);
    int VariantCount(Id nId);
}
