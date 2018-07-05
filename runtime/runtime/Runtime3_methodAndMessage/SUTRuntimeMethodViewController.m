//
//  SUTRuntimeMethodViewController.m
//  runtime
//
//  Created by airende on 2018/7/5.
//  Copyright © 2018 airende. All rights reserved.
//

#import "SUTRuntimeMethodViewController.h"
#import "SUTRuntimeMethodHelper.h"
#import <objc/runtime.h>

@interface SUTRuntimeMethodViewController ()
{
    SUTRuntimeMethodHelper *_helper;
}
@end

@implementation SUTRuntimeMethodViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
    //SUTRuntimeMethodHelper类是一个帮助类，当 performSelector: 调用未实现的方法时进行消息转发，就可以转发到helper类里面的方法中。
    
//    [self test];   //解注释使用
}


+ (instancetype)object {
    return [[self alloc] init];
}

- (instancetype)init {
    self = [super init];
    if (self != nil) {
        _helper = [[SUTRuntimeMethodHelper alloc] init];
    }
    return self;
}

//- (void)test {
//    [self performSelector:@selector(method2)];
//}


#pragma - mark - 1.动态方法解析  method1
/*
 
 - (void)test {
 [self performSelector:@selector(method1)];
 }

 
void functionForMethod1(id self, SEL _cmd) {
    NSLog(@"动态方法解析");
    NSLog(@"%@, %p", self, _cmd);
}

+ (BOOL)resolveInstanceMethod:(SEL)sel {
    NSString *selectorString = NSStringFromSelector(sel);
    if ([selectorString isEqualToString:@"method1"]) {
        class_addMethod(self.class, @selector(method1), (IMP)functionForMethod1, "@:");
    }
    return [super resolveInstanceMethod:sel];
}
*/


#pragma - mark - 2.备用接收者  helperMethod helper类里面的方法。  performSelector:只能调用本类中的 helperMethod 方法，不处理会crash。
/*
 - (void)test {
 [self performSelector:@selector(helperMethod)];
 }
 
- (id)forwardingTargetForSelector:(SEL)aSelector {
    NSLog(@"forwardingTargetForSelector");
    NSString *selectorString = NSStringFromSelector(aSelector);
    // 将消息转发给_helper来处理
    if ([selectorString isEqualToString:@"helperMethod"]) {
        return _helper;
    }
    return [super forwardingTargetForSelector:aSelector];
}
*/

#pragma - mark - 3.完整消息转发 -- 已实现的方法不会走这边。 消息转发顺序 1-2-3

//消息转发机制使用从这个方法中获取的信息来创建NSInvocation对象。因此我们必须重写这个方法，为给定的selector提供一个合适的方法签名。
/*
- (void)test {
    [self performSelector:@selector(helperMethod)];
}

- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector {
    NSMethodSignature *signature = [super methodSignatureForSelector:aSelector];
    if (!signature) {
        //处理未找到的消息，在helper里面找这个方法，若有存在就注册这个方法。然后执行下面代码。
        if ([SUTRuntimeMethodHelper instancesRespondToSelector:aSelector]) {
            signature = [SUTRuntimeMethodHelper instanceMethodSignatureForSelector:aSelector];
        }
    }
    return signature;
}

//如果在上一步还不能处理未知消息，则唯一能做的就是启用完整的消息转发机制了。此时会调用以下方法：
- (void)forwardInvocation:(NSInvocation *)anInvocation {
    if ([SUTRuntimeMethodHelper instancesRespondToSelector:anInvocation.selector]) {
        [anInvocation invokeWithTarget:_helper];
    }
}
*/
@end
