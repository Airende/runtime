//
//  NSObject+ARDKVO.m
//  runtime
//
//  Created by airende on 2017/12/12.
//  Copyright © 2017年 airende. All rights reserved.
//

#import "NSObject+ARDKVO.h"
#import <objc/message.h>

@implementation NSObject (ARDKVO)


- (void)ARD_addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context{
    
    //1.动态生成一个类名
    NSString *oldClassName = NSStringFromClass([self class]);
    NSString *newName = [@"ARD_" stringByAppendingString:oldClassName];
    const char *newClassName = [newName UTF8String];
    
    //2.创建一个类
    Class myClass = objc_allocateClassPair([self class], newClassName, 0);
    
    //3.类添加一个方法
    class_addMethod([self class], @selector(setName:), (IMP)setName, "v@:@");
    
    //4.注册该类
    objc_registerClassPair(myClass);
    
    //5.修改被观察者的isa指针,指向自定义的类
    object_setClass(self, myClass);
    
    //6.动态绑定属性
    objc_setAssociatedObject(self, (__bridge const void *)@"123", observer, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    
    
}

void setName(id self,SEL _cmd,NSString * newName){
    
    //1.保存当前类
    id class = [self class];
    
    //2.
    object_setClass(self, class_getSuperclass([self class]));
    
    //3.调用父类的setName方法
    objc_msgSend(self, @selector(setName:),newName);
    
    //4.拿出观察者
    id observer = objc_getAssociatedObject(self, (__bridge const void *)@"123");
    
    //5.通知
    objc_msgSend(observer, @selector(observeValueForKeyPath:ofObject:change:context:),self,@"name",@{@"new":newName},nil);
    
    //6.改回子类
    object_setClass(self, class);
}

@end
