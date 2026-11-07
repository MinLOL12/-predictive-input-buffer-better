#include "../src/menu_shortcut.hpp"
#include <cassert>

int main() {
    using menu_shortcut::shouldOpen;
    // v2.85: plain F3 opens the menu while in a level (no modifier needed any more).
    assert(shouldOpen(true, true, false, true));
    // F3 is the menu key on its own, and the other shortcuts keep working.
    assert(!shouldOpen(false, true, false, true));
    assert(!shouldOpen(true, false, false, true));
    assert(!shouldOpen(true, true, true, true));
    // Nothing to open outside a level.
    assert(!shouldOpen(true, true, false, false));
}
