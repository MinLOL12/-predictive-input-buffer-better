#include "../src/wave_rescue_policy.hpp"
#include <cassert>

int main() {
    using namespace wave_rescue_policy;
    // Both wave directions must be checked across normal/inverted transitions.
    assert(canRescue(1, true, false, true, false, false));
    assert(canRescue(1, false, true, true, false, false));
    assert(!canRescue(1, false, false, true, false, false));
    assert(!canRescue(2, false, true, true, false, false));
    assert(canRescue(1, true, false, false, false, true));
    assert(!canRescue(1, false, true, false, false, true));
    assert(canRescue(1, false, true, false, true, false));
    assert(!canRescue(1, false, true, false, false, false));

    assert(preserveQueueOnSpam(true, false, false));
    assert(!preserveQueueOnSpam(true, false, true));
    assert(!preserveQueueOnSpam(true, true, false));
    assert(!preserveQueueOnSpam(false, false, false));

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
