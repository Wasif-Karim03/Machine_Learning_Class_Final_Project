# Simple Tkinter Whiteboard

This is a tiny whiteboard app written with Python's built-in Tkinter GUI toolkit. It opens a canvas where you can draw freehand using a mouse or trackpad.

## Features
- Freehand drawing with smooth strokes
- Press and hold the left button to draw, release to lift the pen
- Hit `c` to clear the canvas
- Press `Esc` to close the window

## Prerequisites
- Python 3.8+ with Tkinter support

Tkinter ships with the standard Python installer from [python.org](https://www.python.org/downloads/). If you prefer Homebrew on macOS, add the Tk-enabled Python build:

```bash
brew install python-tk@3.12
brew link --overwrite python-tk@3.12
python3.12 -m tkinter  # quick test that Tk is available
```

On Linux, install your distro's Tk package (e.g., `sudo apt install python3-tk`). On Windows, the official Python installer already bundles Tk.

## Setup
1. Clone the repo or download the files.
2. (Optional) Create and activate a virtual environment:

   ```bash
   python3 -m venv .venv
   source .venv/bin/activate  # Windows: .venv\Scripts\activate
   ```

3. Install dependencies (there are none beyond Tkinter, but this keeps the workflow standard):

   ```bash
   pip install -r requirements.txt
   ```

## Run

```bash
python3 whiteboard.py
```

If you have multiple Python versions installed, call the one that has Tkinter (for example `/Library/Frameworks/Python.framework/Versions/3.12/bin/python3 whiteboard.py`).

## Controls
- Drag with left click / trackpad to draw
- `c` clears the canvas
- `Esc` exits the app


