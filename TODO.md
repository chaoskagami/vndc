What needs to be done
=====

 - Stop using dirty rendering completely.
 - Make a better debug-console/menu system.
   - Implement a proper save menu with highlighting.
   - Also, op_choice. It's terrible and hackish.
 - Change all direct SDL uses to call Zero instead - since that will be backend-independent.
 - Rework op_text to use newlined buffers instead of dirty text renders
   - After reworking op_text, make current text on-screen restored with saves
   - Also, find an inventive way to save screenshots and current sfx.
