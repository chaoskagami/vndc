What needs to be done (now)
-----

 - Rip out this shitty build system in favor of platform-specific makefiles and autotools for everything else. Let's be honest; it sucks and I know it.
 - Remove all remaining direct SDL calls in code, move to zero.
   - I've done heavy overhauling on zero in my private code, so merging it back with fixups will happen first.
 - Port to 3DS. Yes, there's an existing vnds port. It sucks.
   - Namely, it doesn't have support for proper control flow - same as the original VNDS.
 - Port to android. It's about damned time I get things working.

What needs to be done (eventually)
-----

 - Stop using dirty rendering.
 - Make a better debug-console/menu system.
 - Implement a proper save menu.
 - op_choice is terrible and hackish.
 - Rework op_text to use newlined buffers instead of dirty text renders.
   - After reworking op_text, make current text on-screen restored with saves
   - Also, find an inventive way to save screenshots and current sfx.
