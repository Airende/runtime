//
//  NSObject+ARDKVO.h
//  runtime
//
//  Created by airende on 2017/12/12.
//  Copyright © 2017年 airende. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSObject (ARDKVO)

- (void)ARD_addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;

@end
