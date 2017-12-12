//
//  NSURL+ARDURL.m
//  runtime
//
//  Created by airende on 2017/12/12.
//  Copyright © 2017年 airende. All rights reserved.
//

#import "NSURL+ARDURL.h"
#import <objc/runtime.h>

@implementation NSURL (ARDURL)

+(void)load{

    Method oldMethod = class_getClassMethod([NSURL class], @selector(URLWithString:));
    Method newMethod = class_getClassMethod([NSURL class], @selector(ARD_URLWithString:));
    
    method_exchangeImplementations(oldMethod, newMethod);
    
}


+ (instancetype)ARD_URLWithString:(NSString *)URLString{
    
    if ([URLString isEqualToString:@"www.baidu.com"]) {
        NSLog(@"你输的是百度网址");
//        return nil;
    }
    return [self ARD_URLWithString:URLString];
}

@end
