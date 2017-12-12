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

    //获取一个类的方法
    Method oldMethod = class_getClassMethod([NSURL class], @selector(URLWithString:));
    Method newMethod = class_getClassMethod([NSURL class], @selector(ARD_URLWithString:));
    
    //交换两个类的方法实现
    method_exchangeImplementations(oldMethod, newMethod);
    
}

//替换系统方法的自写方法
+ (instancetype)ARD_URLWithString:(NSString *)URLString{
    
    if ([URLString isEqualToString:@"www.baidu.com"]) {
        NSLog(@"你输的是百度网址");
//        return nil;
    }
    //这个方法的实现已经变成了系统的方法
    return [self ARD_URLWithString:URLString];
}

@end
