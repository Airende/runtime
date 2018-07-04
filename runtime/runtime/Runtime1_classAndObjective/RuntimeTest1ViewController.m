//
//  RuntimeTest1ViewController.m
//  runtime
//
//  Created by airende on 2018/7/4.
//  Copyright © 2018 airende. All rights reserved.
//

#import "RuntimeTest1ViewController.h"
#import "MyClass.h"
#import <objc/runtime.h>


@interface RuntimeTest1ViewController ()

@end

@implementation RuntimeTest1ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
//    [self runtimeTest1];
    [self createClass];
    
}

- (void)runtimeTest1{
    
    MyClass *myClass = [[MyClass alloc] init];
    unsigned int outCount = 0;
    Class cls = myClass.class;
    
    //获取类名
    const char *s1 = class_getName(cls);
    NSLog(@"类名：%s",s1); //类名：MyClass
    NSLog(@"----------------------------");
    
    //父类
    const char *s2 = class_getName(class_getSuperclass(class_getSuperclass(cls)));
    NSLog(@"类名：%s",s2); //类名：nil
    NSLog(@"----------------------------");
    
    // 是否是元类
    BOOL b1 = class_isMetaClass(cls);
    NSLog(@"是否元类：%d",b1); //是否元类：0
    NSLog(@"----------------------------");
    
    Class metaClass = objc_getMetaClass(class_getName(cls));
    NSLog(@"%s 的元类是：%s",s1,class_getName(metaClass)); //MyClass 的元类是：MyClass
    NSLog(@"----------------------------");
    
    size_t sz = class_getInstanceSize(cls);
    NSLog(@"实例大小：%zu",sz); //实例大小：48
    NSLog(@"----------------------------");
    
    // 获取整个成员变量列表
    Ivar *ivars = class_copyIvarList(cls, &outCount);
    for (int i = 0; i < outCount; i++) {
        Ivar ivar = ivars[i];
        NSLog(@"成员变量的名称: %s 下标: %d", ivar_getName(ivar), i);
        /*
         成员变量的名称: _instance1 下标: 0
         成员变量的名称: _instance2 下标: 1
         成员变量的名称: _aArray 下标: 2
         成员变量的名称: _aString 下标: 3
         成员变量的名称: _aInteger 下标: 4
         */
    }
    free(ivars);
    NSLog(@"----------------------------");
    
    // 获取类中指定名称实例成员变量的信息
    Ivar string = class_getInstanceVariable(cls, "_aString");
    if (string != NULL) {
        NSLog(@"获取指定成员变量 %s", ivar_getName(string));//获取指定成员变量 _aString
    }
    NSLog(@"----------------------------");
    
    // 属性操作
    objc_property_t *properties = class_copyPropertyList(cls, &outCount);
    for (int i = 0; i < outCount; i++) {
        objc_property_t property = properties[i];
        NSLog(@"属性名称是: %s", property_getName(property));
        /*
         属性名称是: aInteger
         属性名称是: aArray
         属性名称是: aString
         */
    }
    free(properties);
    NSLog(@"----------------------------");
    
    objc_property_t array = class_getProperty(cls, "aArray");
    if (array != NULL) {
        NSLog(@"property %s", property_getName(array));//property aArray
    }
    NSLog(@"----------------------------");
    
    // 方法操作
    Method *methods = class_copyMethodList(cls, &outCount);
    for (int i = 0; i < outCount; i++) {
        Method method = methods[i];
        NSLog(@"方法名: %s", method_getName(method));
        /*
         方法名: method1
         方法名: method2
         方法名: method3WithArg1:arg2:
         方法名: aArray
         方法名: setAArray:
         方法名: aString
         方法名: setAString:
         方法名: aInteger
         方法名: setAInteger:
         方法名: .cxx_destruct
         */
    }
    free(methods);
    NSLog(@"----------------------------");
    
    Method method1 = class_getInstanceMethod(cls, @selector(method1));
    if (method1 != NULL) {
        NSLog(@"方法：%s", method_getName(method1));//方法：method1
    }
    NSLog(@"----------------------------");
    //获取类方法
    Method classMethod = class_getClassMethod(cls, @selector(classMethod1));
    if (classMethod != NULL) {
        NSLog(@"类方法 : %s", method_getName(classMethod));
    }
    
    //去响应某方法
    NSLog(@"MyClass is%@ responsd to selector: method3WithArg1:arg2:", class_respondsToSelector(cls, @selector(method3WithArg1:arg2:)) ? @"" : @" not");
    
    //IMP 方法实现指针
    IMP imp = class_getMethodImplementation(cls, @selector(method1));
    imp;
}

- (void)createClass{
    
//    IMP imp_submethod1 = nil;
    
    Class cls = objc_allocateClassPair(MyClass.class, "MySubClass", 0);
    class_addMethod(cls, @selector(submethod1), (IMP)imp_submethod1, "v@:");
    class_replaceMethod(cls, @selector(method1), (IMP)imp_submethod1, "v@:");
    class_addIvar(cls, "_ivar1", sizeof(NSString *), log(sizeof(NSString *)), "i");
    objc_property_attribute_t type = {"T", "@\"NSString\""};
    objc_property_attribute_t ownership = { "C", "" };
    objc_property_attribute_t backingivar = { "V", "_ivar1"};
    objc_property_attribute_t attrs[] = {type, ownership, backingivar};
    class_addProperty(cls, "property2", attrs, 3);
    objc_registerClassPair(cls);
    id instance = [[cls alloc] init];
    [instance performSelector:@selector(submethod1)];
    [instance performSelector:@selector(method1)];
}

void imp_submethod1(id self, SEL _cmd)
{
    // implementation ....
    NSLog(@"执行方法 imp_submethod1");
}



@end
