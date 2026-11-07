#pragma once

#include <algorithm>

// Policy values are in physics ticks; callers convert 240 Hz constants with st().
namespace prediction_policy {
    // Never reorder a pending input sequence. Otherwise let an intentional press on
    // an orb reach the game unchanged, regardless of the predicted outcome - unless
    // "orb timing" is on and the press may be re-timed inside that same orb.
    constexpr bool passOrbPress(bool down, bool touchingOrb, bool queueEmpty, bool retimeInOrb) {
        return down && touchingOrb && queueEmpty && !retimeInOrb;
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

    constexpr bool strictWaveRecovery(bool startedInverted, bool gravityChanged) {
        return startedInverted || gravityChanged;
    }

    inline int waveMinimumTurn(bool invertedWave, int normalMinimum, int invertedMinimum) {
        return invertedWave ? invertedMinimum : normalMinimum;
    }

    inline int waveEscape(bool invertedWave, int normalEscape, int invertedEscape) {
        return invertedWave ? invertedEscape : normalEscape;
    }

}
