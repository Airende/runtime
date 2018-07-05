//
//  SUTRuntimeMethodHelper.m
//  runtime
//
//  Created by airende on 2018/7/5.
//  Copyright © 2018 airende. All rights reserved.
//

#import "SUTRuntimeMethodHelper.h"

@implementation SUTRuntimeMethodHelper

- (void)helperMethod {
    NSLog(@"执行了helper的方法");
    NSLog(@"%@, %p", self, _cmd);
}

@end
