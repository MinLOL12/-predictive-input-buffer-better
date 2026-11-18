#include "../src/wave_auto_release_policy.hpp"
#include <array>
#include <cassert>

int main() {
    using namespace wave_auto_release_policy;
    constexpr int window = 6, clearance = 73;
    constexpr Prediction hit{3, true, false, false};
    constexpr Prediction clear{clearance, false, false, true};

    auto check = [](Prediction hold, Prediction now, std::array<Prediction, 3> later, int maxProbes = 6) {
        int probes = 0;
        return choose(window, clearance, [&](int horizon, int releaseAt) {
            assert(releaseAt == probes - 1);
            ++probes;
            assert(horizon == (releaseAt < 0 ? window : clearance));
            return releaseAt < 0 ? hold : releaseAt == 0 ? now : later.at(releaseAt - 1);
        }, [&] { return probes >= maxProbes; });
    };

    assert(!check({window, false, false, false}, clear, {})); // holding through the corridor works
    assert(!check({2, false, true, false}, clear, {})); // an interrupted hold isn't a crash
    assert(!check(hit, {14, true, false, true}, {})); // release lives 11 ticks longer but still crashes
    assert(!check(hit, clear, {hit, hit, clear})); // stay held; the last release tick works
    assert(!check(hit, {clearance, false, true, true}, {})); // an unknown portal isn't clearance
    assert(!check(hit, {clearance, false, false, false}, {})); // the release never fired
    assert(!check(hit, clear, {hit, {4, true, true, true}, hit})); // waiting has an uncertain outcome
    assert(!check(hit, clear, {hit, hit, hit}, 3)); // ran out of time to check later releases

    auto save = check(hit, clear, {hit, hit, hit});
    assert(save && save->holdTicks == 3 && save->releaseTicks == clearance);
    assert(check({0, true, false, false}, clear, {})); // collision next step: no time to wait
}
