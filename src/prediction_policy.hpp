#pragma once

#include <algorithm>

// Policy values are in physics ticks; callers convert 240 Hz constants with st().
namespace prediction_policy {
    // Never reorder a pending input sequence. Otherwise let an intentional press on
    // an orb reach the game unchanged, regardless of the predicted outcome.
    constexpr bool passOrbPress(bool down, bool touchingOrb, bool queueEmpty) {
        return down && touchingOrb && queueEmpty;
    }

    template <typename T>
    constexpr bool sameOrb(T const* target, T const* used) {
        return target && target == used;
    }

    constexpr bool betterOrbFallback(bool enabled, bool sameTarget, int survival, int bestSurvival) {
        return enabled && sameTarget && survival > bestSurvival;
    }

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
