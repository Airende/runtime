//
//  MyClass.m
//  runtime
//
//  Created by airende on 2018/7/4.
//  Copyright © 2018 airende. All rights reserved.
//

#import "MyClass.h"

@interface MyClass ()
{
    NSInteger _instance1;
    NSString *_instance2;
}

@property (nonatomic, assign) NSUInteger aInteger;

- (void)method3WithArg1:(NSInteger)arg1 arg2:(NSString *)arg2;

@end

@implementation MyClass

+ (void)classMethod1 {
    NSLog(@"classMethod1");
}
- (void)method1 {
    NSLog(@"call method method1");
}
- (void)method2 {
    NSLog(@"call method method1");
}
- (void)method3WithArg1:(NSInteger)arg1 arg2:(NSString *)arg2 {
    NSLog(@"arg1 : %ld, arg2 : %@", arg1, arg2);
}

@end
