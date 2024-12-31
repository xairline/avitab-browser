#import <Cocoa/Cocoa.h>

extern "C" void setPointingHandCursor() {
    @autoreleasepool {
        [[NSCursor pointingHandCursor] set];
    }
}

extern "C" void setIBeamCursor() {
    @autoreleasepool {
        [[NSCursor IBeamCursor] set];
    }
}
