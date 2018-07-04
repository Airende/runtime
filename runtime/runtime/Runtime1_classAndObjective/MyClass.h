//
//  MyClass.h
//  runtime
//
//  Created by airende on 2018/7/4.
//  Copyright © 2018 airende. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MyClass : NSObject

@property (nonatomic, strong) NSArray *aArray;
@property (nonatomic, copy) NSString *aString;

- (void)method1;
- (void)method2;
+ (void)classMethod1;

@end

NS_ASSUME_NONNULL_END
