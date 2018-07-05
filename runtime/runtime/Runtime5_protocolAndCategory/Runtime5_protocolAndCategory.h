//
//  Runtime5_protocolAndCategory.h
//  runtime
//
//  Created by airende on 2018/7/5.
//  Copyright © 2018 airende. All rights reserved.
//

#ifndef Runtime5_protocolAndCategory_h
#define Runtime5_protocolAndCategory_h


#endif /* Runtime5_protocolAndCategory_h */

源地址：http://southpeak.github.io/2014/11/08/objective-c-runtime-5/

Objective-C中的分类允许我们通过给一个类添加方法来扩充它（但是通过category不能添加新的实例变量），并且我们不需要访问类中的代码就可以做到。

Objective-C中的协议是普遍存在的接口定义方式，即在一个类中通过@protocol定义接口，在另外类中实现接口，这种接口定义方式也成为“delegation”模式，@protocol声明了可以呗其他任何方法类实现的方法，协议仅仅是定义一个接口，而由其他的类去负责实现。


----------------
|* 基础数据类型 *|
----------------

Category
Category是表示一个指向分类的结构体的指针，其定义如下：

    typedef struct objc_category *Category;
    struct objc_category {
        char *category_name                          OBJC2_UNAVAILABLE;    // 分类名
        char *class_name                             OBJC2_UNAVAILABLE;    // 分类所属的类名
        struct objc_method_list *instance_methods    OBJC2_UNAVAILABLE;    // 实例方法列表
        struct objc_method_list *class_methods       OBJC2_UNAVAILABLE;    // 类方法列表
        struct objc_protocol_list *protocols         OBJC2_UNAVAILABLE;    // 分类所实现的协议列表
    }

这个结构体主要包含了分类定义的实例方法与类方法，其中instance_methods列表是objc_class中方法列表的一个子集，而class_methods列表是元类方法列表的一个子集。

--------
Protocol
Protocol的定义如下：
typedef struct objc_object Protocol;
我们可以看到，Protocol其中实就是一个对象结构体。

Runtime并没有在<objc/runtime.h>头文件中提供针对分类的操作函数。因为这些分类中的信息都包含在objc_class中，我们可以通过针对objc_class的操作函数来获取分类的信息。

------
而对于Protocol，runtime提供了一系列函数来对其进行操作，这些函数包括：

// 返回指定的协议
Protocol * objc_getProtocol ( const char *name );
// 获取运行时所知道的所有协议的数组
Protocol ** objc_copyProtocolList ( unsigned int *outCount );
// 创建新的协议实例
Protocol * objc_allocateProtocol ( const char *name );
// 在运行时中注册新创建的协议
void objc_registerProtocol ( Protocol *proto );
// 为协议添加方法
void protocol_addMethodDescription ( Protocol *proto, SEL name, const char *types, BOOL isRequiredMethod, BOOL isInstanceMethod );
// 添加一个已注册的协议到协议中
void protocol_addProtocol ( Protocol *proto, Protocol *addition );
// 为协议添加属性
void protocol_addProperty ( Protocol *proto, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount, BOOL isRequiredProperty, BOOL isInstanceProperty );
// 返回协议名
const char * protocol_getName ( Protocol *p );
// 测试两个协议是否相等
BOOL protocol_isEqual ( Protocol *proto, Protocol *other );
// 获取协议中指定条件的方法的方法描述数组
struct objc_method_description * protocol_copyMethodDescriptionList ( Protocol *p, BOOL isRequiredMethod, BOOL isInstanceMethod, unsigned int *outCount );
// 获取协议中指定方法的方法描述
struct objc_method_description protocol_getMethodDescription ( Protocol *p, SEL aSel, BOOL isRequiredMethod, BOOL isInstanceMethod );
// 获取协议中的属性列表
objc_property_t * protocol_copyPropertyList ( Protocol *proto, unsigned int *outCount );
// 获取协议的指定属性
objc_property_t protocol_getProperty ( Protocol *proto, const char *name, BOOL isRequiredProperty, BOOL isInstanceProperty );
// 获取协议采用的协议
Protocol ** protocol_copyProtocolList ( Protocol *proto, unsigned int *outCount );
// 查看协议是否采用了另一个协议
BOOL protocol_conformsToProtocol ( Protocol *proto, Protocol *other );

注：
    1.objc_getProtocol函数，需要注意的是如果仅仅是声明了一个协议，而未在任何类中实现这个协议，则该函数返回的是nil。
    2.objc_copyProtocolList函数，获取到的数组需要使用free来释放
    3.objc_allocateProtocol函数，如果同名的协议已经存在，则返回nil
    4.objc_registerProtocol函数，创建一个新的协议后，必须调用该函数以在运行时中注册新的协议。协议注册后便可以使用，但不能再做修改，即注册完后不能再向协议添加方法或协议

