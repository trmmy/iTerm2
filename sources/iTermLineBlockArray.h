//
//  iTermLineBlockArray.h
//  iTerm2SharedARC
//
//  Created by George Nachman on 13.10.18.
//

#import <Foundation/Foundation.h>
#import "ScreenCharArray.h"
#import "iTermMetadata.h"

NS_ASSUME_NONNULL_BEGIN

@class LineBlock;
@class iTermLineBlockArray;

@protocol iTermLineBlockArrayDelegate <NSObject>
- (void)lineBlockArrayDidChange:(iTermLineBlockArray *)lineBlockArray;
@end

@interface iTermLineBlockArray : NSObject<NSCopying>

@property (nonatomic, readonly) NSArray<LineBlock *> *blocks;
@property (nonatomic, readonly) NSUInteger count;
@property (nonatomic, readonly) LineBlock *lastBlock;
@property (nonatomic, readonly) LineBlock *firstBlock;
@property (nonatomic) BOOL resizing;
@property (nonatomic, readonly) NSString *dumpForCrashlog;
@property (nonatomic, weak) id<iTermLineBlockArrayDelegate> delegate;

- (NSString *)dumpWidths:(NSSet<NSNumber *> * _Nullable)widths;

// NOTE: Update -copyWithZone: if you add properties.

- (LineBlock *)objectAtIndexedSubscript:(NSUInteger)index;
- (LineBlock *)addBlockOfSize:(int)size
                       number:(long long)number
  mayHaveDoubleWidthCharacter:(BOOL)mayHaveDoubleWidthCharacter;
- (void)addBlock:(LineBlock *)object;
- (void)removeFirstBlock;
- (void)removeFirstBlocks:(NSInteger)count;
- (void)removeLastBlock;
- (void)setAllBlocksMayHaveDoubleWidthCharacters;
- (NSInteger)indexOfBlockContainingLineNumber:(int)lineNumber width:(int)width remainder:(out nonnull int *)remainderPtr;
- (nullable LineBlock *)blockContainingLineNumber:(int)lineNumber
                                            width:(int)width
                                        remainder:(out int *)remainderPtr;
- (int)numberOfWrappedLinesForWidth:(int)width;
- (void)enumerateLinesInRange:(NSRange)range
                        width:(int)width
                        block:(void (^)(const screen_char_t *chars,
                                        int length,
                                        int eol,
                                        screen_char_t continuation,
                                        iTermImmutableMetadata metadata,
                                        BOOL *stop))block;
- (NSInteger)numberOfRawLines;
- (NSInteger)rawSpaceUsed;
- (NSInteger)rawSpaceUsedInRangeOfBlocks:(NSRange)range;

// If you don't need a yoffset pass -1 for width and NULL for blockOffset to avoid building a cache.
- (LineBlock * _Nullable)blockContainingPosition:(long long)p
                                         yOffset:(int)yOffset
                                           width:(int)width
                                       remainder:(nullable int *)remainder
                                     blockOffset:(nullable int *)yoffset
                                           index:(nullable int *)indexPtr;
- (void)sanityCheck:(long long)droppedChars;
- (void)oopsWithWidth:(int)width droppedChars:(long long)droppedChars block:(void (^)(void))block;
- (NSSet<NSNumber *> *)cachedWidths;
- (NSInteger)numberOfWrappedLinesForWidth:(int)width
                          upToBlockAtIndex:(NSInteger)limit;

@end

NS_ASSUME_NONNULL_END
