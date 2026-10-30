#include "../src/menu_shortcut.hpp"
#include <cassert>

int main() {
    using menu_shortcut::shouldOpen;
    // Both left and right Alt are represented by the event's Alt modifier.
    assert(shouldOpen(true, true, false, true, true));
    // Bare F10 remains available to the path-view handler.
    assert(!shouldOpen(true, true, false, false, true));
    assert(!shouldOpen(false, true, false, true, true));
    assert(!shouldOpen(true, false, false, true, true));
    assert(!shouldOpen(true, true, true, true, true));
    assert(!shouldOpen(true, true, false, true, false));
}
