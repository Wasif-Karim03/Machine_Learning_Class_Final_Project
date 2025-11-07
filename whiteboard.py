import tkinter as tk  # tkinter is the built-in GUI toolkit, so no extra installs (usually)


class Whiteboard(tk.Tk):
    def __init__(self):
        super().__init__()  # set up the Tk window stuff first
        self.title("Simple Whiteboard")  # title bar text
        self.geometry("800x600")  # default size that feels roomy enough for drawing

        # the canvas is basically our drawing board
        self.canvas = tk.Canvas(self, bg="white", highlightthickness=0)
        self.canvas.pack(fill=tk.BOTH, expand=True)  # make it stretch with the window

        # keep track of where the last mouse point was so we can connect the dots
        self.last_x = None
        self.last_y = None

        self.bind_events()  # throw all the event hookups in one place so init is cleaner

    def bind_events(self):
        # mouse down starts drawing
        self.canvas.bind("<ButtonPress-1>", self.on_button_press)
        # dragging with left click actually draws the lines
        self.canvas.bind("<B1-Motion>", self.on_draw)
        # when you lift your finger we stop drawing
        self.canvas.bind("<ButtonRelease-1>", self.on_button_release)
        # hit the letter c to wipe the board, like clearing a chalkboard
        self.bind("<KeyPress-c>", self.clear_canvas)
        # escape key just closes the program (no fancy prompts)
        self.bind("<Escape>", lambda _: self.destroy())

    def on_button_press(self, event):
        # remember where the pen first touched down
        self.last_x, self.last_y = event.x, event.y

    def on_draw(self, event):
        if self.last_x is not None and self.last_y is not None:
            # draw a smooth line from the last point to the new point
            self.canvas.create_line(self.last_x, self.last_y, event.x, event.y,
                                    width=4, fill="black", capstyle=tk.ROUND, smooth=True)
        # update last point so the next segment connects properly
        self.last_x, self.last_y = event.x, event.y

    def on_button_release(self, _event):
        # lifting the mouse means we stop drawing until the next press
        self.last_x = None
        self.last_y = None

    def clear_canvas(self, _event=None):
        # delete everything the canvas has drawn so far
        self.canvas.delete("all")


if __name__ == "__main__":
    # create the window and kick off the tkinter event loop
    whiteboard = Whiteboard()
    whiteboard.mainloop()


