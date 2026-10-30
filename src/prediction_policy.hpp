#pragma once

#include <algorithm>

// Policy values are in physics ticks; callers convert 240 Hz constants with st().
namespace prediction_policy {
    template <typename Player>
    bool invertedWave(Player const* p1, Player const* p2, bool dual) {
        return (p1 && p1->m_isDart && p1->m_isUpsideDown) ||
            (dual && p2 && p2->m_isDart && p2->m_isUpsideDown);
    }

    inline int waveSpamGap(int configuredGap, bool invertedWave, int invertedGap) {
        return invertedWave ? std::min(configuredGap, invertedGap) : configuredGap;
    }

    inline int waveMinimumTurn(bool invertedWave, int normalMinimum, int invertedMinimum) {
        return invertedWave ? invertedMinimum : normalMinimum;
    }

    inline int waveEscape(bool invertedWave, int normalEscape, int invertedEscape) {
        return invertedWave ? invertedEscape : normalEscape;
    }

    inline int waveTarget(bool invertedWave, int horizon, int base, int improvement) {
        return invertedWave ? horizon : std::min(horizon, base + improvement);
    }
}
