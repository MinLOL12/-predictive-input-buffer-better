#include "../src/prediction_policy.hpp"
#include <cassert>

struct Player {
    bool m_isDart;
    bool m_isUpsideDown;
};

int main() {
    using namespace prediction_policy;
    assert(passOrbPress(true, true, true));
    assert(!passOrbPress(false, true, true));
    assert(!passOrbPress(true, false, true));
    assert(!passOrbPress(true, true, false)); // keep pending inputs ordered
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
    assert(waveTarget(false, 65, 10, 12) == 22);
    assert(waveTarget(false, 65, 60, 12) == 65);
    assert(waveTarget(true, 65, 10, 12) == 65);
}
