#import <Cocoa/Cocoa.h>

@interface WhiteboardView : NSView
@property (strong) NSBezierPath *path;
@property NSPoint lastPoint;
@end

@implementation WhiteboardView

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (self = [super initWithFrame:frameRect])
    {
        _path = [NSBezierPath bezierPath];
        [_path setLineWidth:6.0];
        [self setWantsLayer:YES];
        self.layer.backgroundColor = NSColor.whiteColor.CGColor;
    }
    return self;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)mouseDown:(NSEvent *)event
{
    NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
    self.lastPoint = location;
    [self.path moveToPoint:location];
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
    [self.path lineToPoint:location];
    self.lastPoint = location;
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *)event
{
    NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
    [self.path lineToPoint:location];
    [self setNeedsDisplay:YES];
}

- (void)keyDown:(NSEvent *)event
{
    if (event.keyCode == 53) // Escape key
    {
        [NSApp terminate:nil];
        return;
    }

    NSString *characters = event.characters.lowercaseString;
    if (characters.length == 1 && [characters characterAtIndex:0] == 'c')
    {
        self.path = [NSBezierPath bezierPath];
        [self.path setLineWidth:6.0];
        [self setNeedsDisplay:YES];
    }
    else
    {
        [super keyDown:event];
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];

    [[NSColor whiteColor] setFill];
    NSRectFill(self.bounds);

    [[NSColor blackColor] setStroke];
    [self.path stroke];
}

@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (strong) NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    NSRect frame = NSMakeRect(0, 0, 800, 600);
    self.window = [[NSWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled |
                                                         NSWindowStyleMaskClosable |
                                                         NSWindowStyleMaskResizable |
                                                         NSWindowStyleMaskMiniaturizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];

    self.window.title = @"Simple Whiteboard - draw with left mouse. Press C to clear, Esc to quit";
    self.window.backgroundColor = NSColor.whiteColor;

    WhiteboardView *view = [[WhiteboardView alloc] initWithFrame:frame];
    self.window.contentView = view;
    [self.window makeFirstResponder:view];
    [self.window center];
    [self.window makeKeyAndOrderFront:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSApplication *app = [NSApplication sharedApplication];
        AppDelegate *delegate = [AppDelegate new];
        app.delegate = delegate;
        return NSApplicationMain(argc, argv);
    }
}


