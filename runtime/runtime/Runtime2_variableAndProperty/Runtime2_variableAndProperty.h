//
//  Runtime2_variableAndProperty.h
//  runtime
//
//  Created by airende on 2018/7/5.
//  Copyright © 2018 airende. All rights reserved.
//

#ifndef Runtime2_variableAndProperty_h
#define Runtime2_variableAndProperty_h


#endif /* Runtime2_variableAndProperty_h */

源地址：http://southpeak.github.io/2014/10/30/objective-c-runtime-2/
---------------
|* 基础数据类型 *|
---------------

Ivar
Ivar是表示实例变量的类型，其实际是一个指向objc_ivar结构体的指针，其定义如下：

typedef struct objc_ivar *Ivar;
struct objc_ivar {
    char *ivar_name                   OBJC2_UNAVAILABLE;    // 变量名
    char *ivar_type                 OBJC2_UNAVAILABLE;    // 变量类型
    int ivar_offset                    OBJC2_UNAVAILABLE;    // 基地址偏移字节
#ifdef __LP64__
    int space                         OBJC2_UNAVAILABLE;
#endif
}
------------

objc_property_t
objc_property_t是表示Objective-C声明的属性的类型，其实际是指向objc_property结构体的指针，其定义如下：

typedef struct objc_property *objc_property_t;
-----------

objc_property_attribute_t
objc_property_attribute_t定义了属性的特性(attribute)，它是一个结构体，定义如下：

typedef struct {
    const char *name;           // 特性名
    const char *value;          // 特性值
} objc_property_attribute_t;
-----------

-------------------------------
|* 关联对象(Associated Object) *|
-------------------------------

关联对象类似于成员变量，不过是在运行时添加的。我们通常会把成员变量(Ivar)放在类声明的头文件中，或者放在类实现的@implementation后面。但这有一个缺点，我们不能在分类中添加成员变量。如果我们尝试在分类中添加新的成员变量，编译器会报错。

这个内存管理的策略可以由以下值指定：
OBJC_ASSOCIATION_ASSIGN
OBJC_ASSOCIATION_RETAIN_NONATOMIC
OBJC_ASSOCIATION_COPY_NONATOMIC
OBJC_ASSOCIATION_RETAIN
OBJC_ASSOCIATION_COPY

我们将一个对象连接到其它对象所需要做的就是下面两行代码：
static char myKey;
objc_setAssociatedObject(self, &myKey, anObject, OBJC_ASSOCIATION_RETAIN);

关联对象
关联对象操作函数包括以下：
// 设置关联对象
void objc_setAssociatedObject ( id object, const void *key, id value, objc_AssociationPolicy policy );
// 获取关联对象
id objc_getAssociatedObject ( id object, const void *key );
// 移除关联对象
void objc_removeAssociatedObjects ( id object );

属性
属性操作相关函数包括以下：
// 获取属性名
const char * property_getName ( objc_property_t property );
// 获取属性特性描述字符串
const char * property_getAttributes ( objc_property_t property );
// 获取属性中指定的特性
char * property_copyAttributeValue ( objc_property_t property, const char *attributeName );
// 获取属性的特性列表
objc_property_attribute_t * property_copyAttributeList ( objc_property_t property, unsigned int *outCount );

注：
    1.property_copyAttributeValue函数，返回的char *在使用完后需要调用free()释放。
    2.property_copyAttributeList函数，返回值在使用完后需要调用free()释放。
