#include "../src/prediction_policy.hpp"
#include <cassert>

struct Player {
    bool m_isDart;
    bool m_isUpsideDown;
};

int main2();

int main() {
    using namespace prediction_policy;
    assert(passOrbPress(true, true, true, false));
    assert(!passOrbPress(false, true, true, false));
    assert(!passOrbPress(true, false, true, false));
    assert(!passOrbPress(true, true, false, false)); // keep pending inputs ordered
    // v2.86 "orb timing": a press inside an orb is re-timed instead of passed through.
    assert(!passOrbPress(true, true, true, true));
    int orb1 = 1, orb2 = 2;
    assert(sameOrb(&orb1, &orb1));
    assert(!sameOrb(&orb1, &orb2));
    assert(!sameOrb(static_cast<int*>(nullptr), static_cast<int*>(nullptr)));
    assert(betterOrbFallback(true, true, 5, -1)); // unsafe can still activate the intended orb
    assert(!betterOrbFallback(true, false, 100, -1)); // never switch targets
    assert(!betterOrbFallback(false, true, 5, -1));
    assert(!betterOrbFallback(true, true, 5, 5));

    Player uprightWave{true, false};
    Player invertedWavePlayer{true, true};
    Player invertedCube{false, true};
    assert(!invertedWave(&uprightWave, &invertedWavePlayer, false));
    assert(invertedWave(&uprightWave, &invertedWavePlayer, true));
    assert(invertedWave(&invertedWavePlayer, &uprightWave, false));
    assert(!invertedWave(&invertedCube, &uprightWave, true));
    assert(!invertedWave(static_cast<Player*>(nullptr), &uprightWave, true));

    assert(waveSpamGap(16, false, 6) == 16);
    assert(waveSpamGap(16, true, 6) == 6);
    assert(waveSpamGap(4, true, 6) == 4); // respect a smaller user setting
    assert(waveMinimumTurn(false, 6, 12) == 6);
    assert(waveMinimumTurn(true, 6, 12) == 12);
    assert(waveEscape(false, 16, 24) == 16);
    assert(waveEscape(true, 16, 24) == 24);
    assert(!strictWaveRecovery(false, false));
    assert(strictWaveRecovery(true, false));
    assert(strictWaveRecovery(false, true));
    assert(strictWaveRecovery(true, true));
    // The late collision at t50 used to be invisible to a +28t recovery
    // check starting at t18. Nor is a single exact follow-up reaction room.
    constexpr int horizon = 88;
    auto lateCollision = [](int, int length) { return std::min(50, length); };
    assert(!viableUfoReflap(18, 12, 24, horizon, lateCollision));
    assert(!viableUfoReflap(18, 12, 24, horizon, [](int at, int length) { return at == 18 ? length : 24; }));
    auto clears = [](int at, int length) { return at == 18 || at == 19 ? length : 24; };
    assert(viableUfoReflap(18, 18, 24, horizon, clears));
    assert(!viableUfoReflap(18, 19, 24, horizon, clears)); // would cut the original hold short
    assert(!viableUfoReflap(18, 12, 20, horizon, clears)); // neighbor presses at the crash
    return main2();
}

// v2.94: ground orb intent, ship spam exemption, wave spam streak, ship straight band.
int main2() {
    using namespace prediction_policy;
    assert(groundOrbIntent(3, GROUND_ORB_REACH));
    assert(!groundOrbIntent(5, GROUND_ORB_REACH));
    assert(!groundOrbIntent(-1, GROUND_ORB_REACH));
    assert(spamStreak(true, 3) == 5);
    assert(spamStreak(false, 3) == 3);
    assert(leaveSafeInput(true, true));
    assert(!leaveSafeInput(true, false));
    assert(!leaveSafeInput(false, true));
    // v2.94: the old 48-unit band retimed ordinary feather recovery as "wobbly".
    assert(straightBand(true) < straightBand(false));
    assert(straightBand(true) >= 60.f);
    assert(straightBand(false) >= 90.f);
    return 0;
}
