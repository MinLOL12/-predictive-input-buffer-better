#include "../src/prediction_policy.hpp"
#include <cassert>

struct Player {
    bool m_isDart;
    bool m_isUpsideDown;
};

int main() {
    using namespace prediction_policy;
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
