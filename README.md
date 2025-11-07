# Simple Whiteboard

This repository now includes two whiteboard implementations:

- `white_board.cpp`: original Win32 + GDI version for Windows.
- `white_board_mac.mm`: native Cocoa version for macOS.

## macOS Build & Run

1. Install Xcode command line tools if you have not already:

   ```bash
   xcode-select --install
   ```

2. Compile the macOS build:

   ```bash
   clang++ white_board_mac.mm -framework Cocoa -o whiteboard
   ```

3. Launch the app:

   ```bash
   ./whiteboard
   ```

Use the left mouse button to draw. Press `C` to clear the canvas and `Esc` to quit.


