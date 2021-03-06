//
//  IDEKit_Breakpoint.h
//  IDEKit
//
//  Created by Glenn Andreas on 9/19/04.
//  Copyright 2004 by Glenn Andreas.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//  
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#import <Cocoa/Cocoa.h>

/*
 Breakpoints are unique objects that may be shared between multiple views (for example, editor and debugger).
 They do, however, "belong" to a single "master" file (even if displayed in different views).  Associated with
 them are projects/target info (not supported yet), it's kind & flags, and optional generic data (for example,
 conditional breakpoints may have their condition stored there).
 
 A breakpoint can be asked what line it lives in, but must be given a context of that - either a specific
 view or a file (which isn't in a view).  In general, the view is the authority (since the view may be editing
 the file).  It just asks the view to go through and find it - pretty simple.  Otherwise, it has a "best line
 num" used when that file isn't open (consider the case of setting a breakpoint in a library file but wanting
 to display what the breakpoints are when that library file isn't open).
 
 */
enum {
    // low 8 bits are the kind
    IDEKit_kNoBreakPoint,
    IDEKit_kBreakPoint,
    // some common "custom" styles of breakpoints
    IDEKit_kBreakPointTracePoint, // print out something and continue
    IDEKit_kBreakPointPause, // update the debugger display for here, and continue
    IDEKit_kBreakPointConditional, // only stop if expression evaulates
    IDEKit_kBreakPointSoundPoint, // play a sound and continue
    IDEKit_kBreakPointSkipPoint, // skip this line

    IDEKit_kBeyondBreakPoint = 100, // past the end of the file
    IDEKit_kBreakPointNotPossible, // in something that couldn't have breakpoint (comment, blank line)
    IDEKit_kBreakpointProgramCounter, // not a breakpoint, but current instruction counter
    IDEKit_kFirstCustomBreakPoint = 128, // and everything after is custom break point as well
    IDEKit_kBreakPointKindMask = 0xff,
    // and then 8 bits of flags
    IDEKit_kDisabledBreakpointFlag = 0x100,
    // and 16 bits reserved
};

extern NSString *IDEKit_BreakpointFile;
extern NSString *IDEKit_BreakpointLineNum;
extern NSString *IDEKit_BreakpointKind;
extern NSString *IDEKit_BreakpointData;
extern NSString *IDEKit_BreakpointProject;
extern NSString *IDEKit_BreakpointTarget;
extern NSString *IDEKit_BreakpointUUID;

enum {
    IDEKit_kBreapointNotFound = 0 // use line 0 to mean "not found"
};
// Breakpoints have unique ids associated with them, so that you can take a file, set a breakpoint,
// run it in the debuggr, close the file, reopen the file later, and have (literally) the same breakpoints
@class IDEKit_UniqueID;
@interface IDEKit_Breakpoint : NSObject {
    IDEKit_UniqueID *myUnique;
    IDEKit_UniqueID *myFile; // what file does this come from
    IDEKit_UniqueID *myProj; // not yet supported
    NSString *myTarget; // not yet supported
    NSInteger myBestLineNum; // where were we last seen in myFile (is this even worth holding onto?)
    id myData; // whatever data is associated with us
    NSInteger myKind; // includes the flags from above
}
+ (IDEKit_Breakpoint *) breakpointAssociatedWith:(IDEKit_UniqueID *)bpID;
+ (IDEKit_Breakpoint *) breakpointFromPlist:(NSDictionary *)plist; // will return existing one if possible
- (id) initFromPlist: (NSDictionary *)plist;
- (id) initWithKind: (NSInteger) kind file: (IDEKit_UniqueID *)fileID line: (NSInteger) line;
- (NSDictionary *) asPlist;
- (BOOL) disabled;
- (void) setDisabled: (BOOL) disabled;
- (NSInteger) kind;
- (void) setKind: (NSInteger) kind;
- (NSInteger) lineForBuffer: (IDEKit_UniqueID *) bufferID; // for a given view or file
- (void) setFileLine: (NSInteger) line; // for the master file
- (id) data;
- (void) setData: (id) data;
- (IDEKit_UniqueID *) uniqueID; // breakpoints have unique id
- (IDEKit_UniqueID *) fileID;
- (IDEKit_UniqueID *) projID;
- (NSString *) target;
- (void) drawAtX: (float) midx y: (float) midy;

@end
