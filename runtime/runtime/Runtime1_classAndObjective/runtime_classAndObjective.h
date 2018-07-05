//
//  runtime_classAndObjective.h
//  runtime
//
//  Created by airende on 2018/7/4.
//  Copyright © 2018 airende. All rights reserved.
//

#ifndef runtime_classAndObjective_h
#define runtime_classAndObjective_h


#endif /* runtime_classAndObjective_h */

源地址：http://southpeak.github.io/2014/10/25/objective-c-runtime-1/


Objective-C类是由Class类型来表示的，它实际上是一个指向objc_class结构体的指针。它的定义如下：

typedef struct objc_class *Class;

查看 objc/runtime.h 中 objc_class 结构体的定义如下：

    struct objc_class {
        Class isa  OBJC_ISA_AVAILABILITY;
    #if !__OBJC2__
        Class super_class                       OBJC2_UNAVAILABLE;    // 父类
        const char *name                          OBJC2_UNAVAILABLE;    // 类名
        long version                              OBJC2_UNAVAILABLE;    // 类的版本信息，默认为0
        long info                                OBJC2_UNAVAILABLE;    // 类信息，供运行期使用的一些位标识
        long instance_size                       OBJC2_UNAVAILABLE;    // 该类的实例变量大小
        struct objc_ivar_list *ivars             OBJC2_UNAVAILABLE;    // 该类的成员变量链表
        struct objc_method_list *methodLists     OBJC2_UNAVAILABLE;    // 方法定义的链表
        struct objc_cache *cache                  OBJC2_UNAVAILABLE;    // 方法缓存
        struct objc_protocol_list *protocols     OBJC2_UNAVAILABLE;    // 协议链表
    #endif
    } OBJC2_UNAVAILABLE;

1.isa：需要注意的是在Objective-C中，所有的类自身也是一个对象，这个对象的Class里面也有一个isa指针，它指向/*metaClass(元类)*/，我们会在后面介绍它。
2.super_class：指向该类的父类，如果该类已经是最顶层的根类(如NSObject或NSProxy)，则super_class为NULL。

3.cache：用于缓存最近使用的方法。一个接收者对象接收到一个消息时，它会根据isa指针去查找能够响应这个消息的对象。在实际使用中，这个对象只有一部分方法是常用的，很多方法其实很少用或者根本用不上。这种情况下，如果每次消息来时，我们都是methodLists中遍历一遍，性能势必很差。这时，cache就派上用场了。在我们每次调用过一个方法后，这个方法就会被缓存到cache列表中，下次调用的时候runtime就会优先去cache中查找，如果cache没有，才去methodLists中查找方法。这样，对于那些经常用到的方法的调用，但提高了调用的效率。

4.version：我们可以使用这个字段来提供类的版本信息。这对于对象的序列化非常有用，它可是让我们识别出不同类定义版本中实例变量布局的改变。
----------------
|* objc_cache *|
----------------
上面提到了objc_class结构体中的cache字段，它用于缓存调用过的方法。这个字段是一个指向objc_cache结构体的指针，其定义如下：

    struct objc_cache {
        unsigned int mask /* total = mask + 1 */                 OBJC2_UNAVAILABLE;
        unsigned int occupied                                    OBJC2_UNAVAILABLE;
        Method buckets[1]                                        OBJC2_UNAVAILABLE;
    };

1.mask：一个整数，指定分配的缓存bucket的总数。在方法查找过程中，Objective-C runtime使用这个字段来确定开始线性查找数组的索引位置。指向方法selector的指针与该字段做一个AND位操作(index = (mask & selector))。这可以作为一个简单的hash散列算法。

2.occupied：一个整数，指定实际占用的缓存bucket的总数。

3.buckets：指向Method数据结构指针的数组。这个数组可能包含不超过mask+1个元素。需要注意的是，指针可能是NULL，表示这个缓存bucket没有被占用，另外被占用的bucket可能是不连续的。这个数组可能会随着时间而增长。
---------------------
|* 元类(Meta Class) *|
---------------------

meta-class是一个类对象的类。

当我们向一个对象发送消息时，runtime会在这个对象所属的这个类的方法列表中查找方法；而向一个类发送消息时，会在这个类的meta-class的方法列表中查找。

meta-class之所以重要，是因为它存储着一个类的所有类方法。每个类都会有一个单独的meta-class，因为每个类的类方法基本不可能完全相同。

/- 再深入一下，meta-class也是一个类，也可以向它发送一个消息，那么它的isa又是指向什么呢？为了不让这种结构无限延伸下去，Objective-C的设计者让所有的meta-class的isa指向基类的meta-class，以此作为它们的所属类。即，任何NSObject继承体系下的meta-class都使用NSObject的meta-class作为自己的所属类，而基类的meta-class的isa指针是指向它自己。这样就形成了一个完美的闭环。 -/

-------------------
|* 类与对象操作函数 *|
-------------------
 runtime提供了大量的函数来操作类与对象。类的操作方法大部分是以class_为前缀的，而对象的操作方法大部分是以objc_或object_为前缀。


//!类相关操作函数
我们可以回过头去看看objc_class的定义，runtime提供的操作类的方法主要就是针对这个结构体中的各个字段的。

    struct objc_class {
        Class isa  OBJC_ISA_AVAILABILITY;
    #if !__OBJC2__
        Class super_class                       OBJC2_UNAVAILABLE;    // 父类
        const char *name                          OBJC2_UNAVAILABLE;    // 类名
        long version                              OBJC2_UNAVAILABLE;    // 类的版本信息，默认为0
        long info                                OBJC2_UNAVAILABLE;    // 类信息，供运行期使用的一些位标识
        long instance_size                       OBJC2_UNAVAILABLE;    // 该类的实例变量大小
        struct objc_ivar_list *ivars             OBJC2_UNAVAILABLE;    // 该类的成员变量链表
        struct objc_method_list **methodLists     OBJC2_UNAVAILABLE;    // 方法定义的链表
        struct objc_cache *cache                  OBJC2_UNAVAILABLE;    // 方法缓存
        struct objc_protocol_list *protocols     OBJC2_UNAVAILABLE;    // 协议链表
    #endif
        } OBJC2_UNAVAILABLE;
-----
类名(name)  注意返回值
// 获取类的类名
const char * class_getName ( Class cls );
注：对于class_getName函数，如果传入的cls为Nil，则返回一个字符串。
-----
父类(super_class)和元类(meta-class)
// 获取类的父类
Class class_getSuperclass ( Class cls );
// 判断给定的Class是否是一个元类
BOOL class_isMetaClass ( Class cls );
注：
    1.class_getSuperclass函数，当cls为Nil或者cls为根类时，返回Nil。不过通常我们可以使用NSObject类的superclass方法来达到同样的目的。
    2.class_isMetaClass函数，如果是cls是元类，则返回YES；如果否或者传入的cls为Nil，则返回NO。
-----
实例变量大小(instance_size)
// 获取实例大小
size_t class_getInstanceSize ( Class cls );
-----
成员变量(ivars)及属性 <？后面的参数怎么传>
说明：在objc_class中，所有的成员变量、属性的信息是放在链表ivars中的。ivars是一个数组，数组中每个元素是指向Ivar(变量信息)的指针。runtime提供了丰富的函数来操作这一字段。
1 >成员变量操作函数，主要包含以下函数：
// 获取类中指定名称实例成员变量的信息
Ivar class_getInstanceVariable ( Class cls, const char *name );//const char *name :指定的参数名 "_aString"
// 获取类成员变量的信息
Ivar class_getClassVariable ( Class cls, const char *name );
// 添加成员变量
BOOL class_addIvar ( Class cls, const char *name, size_t size, uint8_t alignment, const char *types );
// 获取整个成员变量列表
Ivar * class_copyIvarList ( Class cls, unsigned int *outCount );//unsigned int *outCount :列表的数量，定义 unsigned int outCount = 0， 使用 &outCount

注：
    1.class_getInstanceVariable函数，它返回一个指向包含name指定的成员变量信息的objc_ivar结构体的指针(Ivar)。
    2.class_getClassVariable函数，目前没有找到关于Objective-C中类变量的信息，一般认为Objective-C不支持类变量。注意，返回的列表不包含父类的成员变量和属性。
    3.Objective-C不支持往已存在的类中添加实例变量，因此不管是系统库提供的提供的类，还是我们自定义的类，都无法动态添加成员变量。
      补充：但如果我们通过运行时来创建一个类的话，又应该如何给它添加成员变量呢？这时我们就可以使用class_addIvar函数了。不过需要注意的是，这个方法只能在objc_allocateClassPair函数与objc_registerClassPair之间调用。另外，这个类也不能是元类。成员变量的按字节最小对齐量是1<<alignment。这取决于ivar的类型和机器的架构。如果变量的类型是指针类型，则传递log2(sizeof(pointer_type))。
    4.class_copyIvarList函数，它返回一个指向成员变量信息的数组，数组中每个元素是指向该成员变量信息的objc_ivar结构体的指针。这个数组不包含在父类中声明的变量。outCount指针返回数组的大小。需要注意的是，我们必须使用free()来释放这个数组。

2 >属性操作函数，主要包含以下函数：
// 获取指定的属性
objc_property_t class_getProperty ( Class cls, const char *name );  //返回值 objc_property_t  具体的某个参数
// 获取属性列表
objc_property_t * class_copyPropertyList ( Class cls, unsigned int *outCount ); //objc_property_t * 一个参数数组，遍历获取 objc_property_t
// 为类添加属性
BOOL class_addProperty ( Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount );
// 替换类的属性
void class_replaceProperty ( Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount );

这一种方法也是针对ivars来操作，不过只操作那些是属性的值。我们在后面介绍属性时会再遇到这些函数。
-----
方法(methodLists)
// 添加方法
BOOL class_addMethod ( Class cls, SEL name, IMP imp, const char *types );
// 获取实例方法
Method class_getInstanceMethod ( Class cls, SEL name );
// 获取类方法
Method class_getClassMethod ( Class cls, SEL name );
// 获取所有方法的数组
Method * class_copyMethodList ( Class cls, unsigned int *outCount );
// 替代方法的实现
IMP class_replaceMethod ( Class cls, SEL name, IMP imp, const char *types );
// 返回方法的具体实现
IMP class_getMethodImplementation ( Class cls, SEL name );
IMP class_getMethodImplementation_stret ( Class cls, SEL name );
// 类实例是否响应指定的selector
BOOL class_respondsToSelector ( Class cls, SEL sel );
注：
    1.class_addMethod的实现会覆盖父类的方法实现，但不会取代本类中已存在的实现，如果本类中包含一个同名的实现，则函数会返回NO。如果要修改已存在实现，可以使用method_setImplementation。一个Objective-C方法是一个简单的C函数，它至少包含两个参数–self和_cmd。所以，我们的实现函数(IMP参数指向的函数)至少需要两个参数，如下所示：
    void myMethodIMP(id self, SEL _cmd)
    {
        // implementation ....
    }
    与成员变量不同的是，我们可以为类动态添加方法，不管这个类是否已存在。

    另外，参数types是一个描述传递给方法的参数类型的字符数组，这就涉及到类型编码，我们将在后面介绍。
    2.class_getInstanceMethod、class_getClassMethod函数，与class_copyMethodList不同的是，这两个函数都会去搜索父类的实现。
    3.class_replaceMethod函数，该函数的行为可以分为两种：如果类中不存在name指定的方法，则类似于class_addMethod函数一样会添加方法；如果类中已存在name指定的方法，则类似于method_setImplementation一样替代原方法的实现。
    4.class_getMethodImplementation函数，该函数在向类实例发送消息时会被调用，并返回一个指向方法实现函数的指针。这个函数会比method_getImplementation(class_getInstanceMethod(cls, name))更快。返回的函数指针可能是一个指向runtime内部的函数，而不一定是方法的实际实现。例如，如果类实例无法响应selector，则返回的函数指针将是运行时消息转发机制的一部分。
    5.class_respondsToSelector函数，我们通常使用NSObject类的respondsToSelector:或instancesRespondToSelector:方法来达到相同目的。
------
协议(objc_protocol_list)

// 添加协议
BOOL class_addProtocol ( Class cls, Protocol *protocol );
// 返回类是否实现指定的协议
BOOL class_conformsToProtocol ( Class cls, Protocol *protocol );
// 返回类实现的协议列表
Protocol * class_copyProtocolList ( Class cls, unsigned int *outCount );

z注：
    1.class_conformsToProtocol函数可以使用NSObject类的conformsToProtocol:方法来替代。
    2.class_copyProtocolList函数返回的是一个数组，在使用后我们需要使用free()手动释放。
-----
版本(version)

// 获取版本号
int class_getVersion ( Class cls );
// 设置版本号
void class_setVersion ( Class cls, int version );

--------------
|* 动态创建类 *|
--------------
// 创建一个新类和元类
Class objc_allocateClassPair ( Class superclass, const char *name, size_t extraBytes );
// 销毁一个类及其相关联的类
void objc_disposeClassPair ( Class cls );
// 在应用中注册由objc_allocateClassPair创建的类
void objc_registerClassPair ( Class cls );
注：
    1.objc_allocateClassPair函数：如果我们要创建一个根类，则superclass指定为Nil。extraBytes通常指定为0，该参数是分配给类和元类对象尾部的索引ivars的字节数。
    实例方法和实例变量应该添加到类自身上，而类方法应该添加到类的元类上。
    2.objc_disposeClassPair函数用于销毁一个类，不过需要注意的是，如果程序运行中还存在类或其子类的实例，则不能调用针对类调用该方法。

在前面介绍元类时，我们已经有接触到这几个函数了，在此我们再举个实例来看看这几个函数的使用。
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

---------------
|* 动态创建对象 *| /* ARC环境下无法使用 */
---------------
// 创建类实例
id class_createInstance ( Class cls, size_t extraBytes );
// 在指定位置创建类实例
id objc_constructInstance ( Class cls, void *bytes );
// 销毁类实例
void * objc_destructInstance ( id obj );

注：
    1.class_createInstance函数：创建实例时，会在默认的内存区域为类分配内存。extraBytes参数表示分配的额外字节数。这些额外的字节可用于存储在类定义中所定义的实例变量之外的实例变量。该函数在ARC环境下无法使用。
    2.objc_constructInstance函数：在指定的位置(bytes)创建类实例。
    3.objc_destructInstance函数：销毁一个类的实例，但不会释放并移除任何与其相关的引用。

---------------
|* 实例操作函数 *|
---------------
1 >针对整个对象进行操作的函数，这类函数包含
// 返回指定对象的一份拷贝
id object_copy ( id obj, size_t size );
// 释放指定对象占用的内存
id object_dispose ( id obj );

    有这样一种场景，假设我们有类A和类B，且类B是类A的子类。类B通过添加一些额外的属性来扩展类A。现在我们创建了一个A类的实例对象，并希望在运行时将这个对象转换为B类的实例对象，这样可以添加数据到B类的属性中。这种情况下，我们没有办法直接转换，因为B类的实例会比A类的实例更大，没有足够的空间来放置对象。此时，我们就要以使用以上几个函数来处理这种情况，如下代码:
        NSObject *a = [[NSObject alloc] init];
        id newB = object_copy(a, class_getInstanceSize(MyClass.class));
        object_setClass(newB, MyClass.class);
        object_dispose(a);

2 >针对对象实例变量进行操作的函数，这类函数包含：
// 修改类实例的实例变量的值
Ivar object_setInstanceVariable ( id obj, const char *name, void *value );
// 获取对象实例变量的值
Ivar object_getInstanceVariable ( id obj, const char *name, void **outValue );
// 返回指向给定对象分配的任何额外字节的指针
void * object_getIndexedIvars ( id obj );
// 返回对象中实例变量的值
id object_getIvar ( id obj, Ivar ivar );
// 设置对象中实例变量的值
void object_setIvar ( id obj, Ivar ivar, id value );

如果实例变量的Ivar已经知道，那么调用object_getIvar会比object_getInstanceVariable函数快，相同情况下，object_setIvar也比object_setInstanceVariable快。

3 >针对对象的类进行操作的函数，这类函数包含：
// 返回给定对象的类名
const char * object_getClassName ( id obj );
// 返回对象的类
Class object_getClass ( id obj );
// 设置对象的类
Class object_setClass ( id obj, Class cls );

-------------
|* 获取类定义 *|
-------------
Objective-C动态运行库会自动注册我们代码中定义的所有的类。我们也可以在运行时创建类定义并使用objc_addClass函数来注册它们。runtime提供了一系列函数来获取类定义相关的信息，这些函数主要包括：
// 获取已注册的类定义的列表
int objc_getClassList ( Class *buffer, int bufferCount );
// 创建并返回一个指向所有已注册类的指针列表
Class * objc_copyClassList ( unsigned int *outCount );
// 返回指定类的类定义
Class objc_lookUpClass ( const char *name );
Class objc_getClass ( const char *name );
Class objc_getRequiredClass ( const char *name );
// 返回指定类的元类
Class objc_getMetaClass ( const char *name );
注：
    1.objc_getClassList函数：获取已注册的类定义的列表。我们不能假设从该函数中获取的类对象是继承自NSObject体系的，所以在这些类上调用方法是，都应该先检测一下这个方法是否在这个类中实现。
    2.获取类定义的方法有三个：objc_lookUpClass, objc_getClass和objc_getRequiredClass。如果类在运行时未注册，则objc_lookUpClass会返回nil，而objc_getClass会调用类处理回调，并再次确认类是否注册，如果确认未注册，再返回nil。而objc_getRequiredClass函数的操作与objc_getClass相同，只不过如果没有找到类，则会杀死进程。
    3.objc_getMetaClass函数：如果指定的类没有注册，则该函数会调用类处理回调，并再次确认类是否注册，如果确认未注册，再返回nil。不过，每个类定义都必须有一个有效的元类定义，所以这个函数总是会返回一个元类定义，不管它是否有效。

