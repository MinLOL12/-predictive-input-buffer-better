#include "../src/wave_rescue_policy.hpp"
#include <cassert>

int main() {
    using namespace wave_rescue_policy;
    // Ship and robot still use the original two-edge prediction.
    assert(canRescuePair(1, true, false, false, true));
    assert(!canRescuePair(1, false, true, false, true));
    assert(canRescuePair(1, false, true, true, false));
    assert(!canRescuePair(1, false, true, false, false));
    assert(!canRescuePair(2, true, false, true, false));
    assert(!usesSpamGuard(true, false, false));
    assert(!usesSpamGuard(true, false, true)); // a dual ball/UFO doesn't disable wave timing
    assert(usesSpamGuard(true, true, false));
    assert(usesSpamGuard(false, false, true));

    Rhythm rhythm;
    rhythm.observe(true, 999, 16);
    rhythm.observe(false, 3, 16);
    assert(!rhythm.tight());
    rhythm.observe(true, 7, 16);
    assert(rhythm.tight());
    assert(rhythm.interval(true) == 3 && rhythm.interval(false) == 7);
    rhythm.observe(false, 0, 16);
    assert(rhythm.interval(true) == 1);
    rhythm.observe(true, 17, 16);
    assert(!rhythm.tight() && rhythm.interval(false) == 0 && rhythm.interval(true) == 0);

    // Two pending edges plus a third can't all land on the same tick.
    int first = 4;
    int second = nextTurnTick(first, 4);
    int earliest = nextTurnTick(second, 4);
    assert(second == 5 && earliest == 6);
    assert(nextTurnTick(4, 8) == 8); // don't shorten an already spaced interval
    auto withinBudget = [] { return false; };
    auto predicts = [](int survivor, int safeTick) {
        return [=](int tick, int horizon) { return tick == safeTick ? horizon : survivor; };
    };
    assert(!retimeWaveTurn(8, earliest, 12, 4, predicts(5, 6), withinBudget)); // prefix already crashes
    assert(!retimeWaveTurn(8, earliest, 12, 4, predicts(-1, 6), withinBudget)); // unknown portal / mover
    assert(!retimeWaveTurn(8, earliest, 12, 4, predicts(9, 8), withinBudget)); // keep a valid rhythm
    assert(retimeWaveTurn(8, earliest, 12, 4, predicts(6, 6), withinBudget) == 6); // turn on the crash tick
    assert(retimeWaveTurn(8, earliest, 12, 4, predicts(9, 9), withinBudget) == 9); // a tick later clears the corner
    assert(!retimeWaveTurn(8, earliest, 8, 4, predicts(9, 9), withinBudget)); // respect stage limit
    assert(!retimeWaveTurn(8, earliest, 12, 4, predicts(9, 0), withinBudget)); // surviving only briefly isn't a save
    assert(!retimeWaveTurn(8, earliest, 12, 4, predicts(6, 6), [] { return true; }));

    // A lookahead ending before or right after the second turn is not a save.
    assert(lookahead(50, 55, 20) == 75);
    assert(lookahead(80, 30, 20) == 80);
    assert(portalIsSafe(false, 5, 5));
    assert(!portalIsSafe(false, 4, 5));
    assert(!portalIsSafe(true, 20, 5)); // unknown portal cannot prove clearance
    assert(diedBeforeNextTurn(54, 55));
    assert(!diedBeforeNextTurn(55, 55)); // death on the turn tick can still be fixed by delaying it
    assert(!clearsNextTurn(true, 55, 55, 20));
    assert(!clearsNextTurn(true, 74, 55, 20));
    assert(clearsNextTurn(true, 75, 55, 20));
    assert(!clearsNextTurn(false, 75, 55, 20)); // delayed dash release never fired

    GravityPortalTracker<int> portals;
    int portal = 1, other = 2;
    assert(portals.activate(&portal, false));
    assert(!portals.activate(&portal, false));
    assert(portals.activate(&portal, true)); // dual ghosts are independent
    assert(!portals.activate(&portal, true));
    assert(portals.activate(&other, false));
    assert(!portals.activate(nullptr, false));
    portals.clear();
    assert(portals.activate(&portal, false));
    assert(portals.activate(&portal, true));
}
