//
//  ViewController.m
//  runtime
//
//  Created by airende on 2017/12/12.
//  Copyright © 2017年 airende. All rights reserved.
//

#import "ViewController.h"
#import "Person.h"
#import <objc/message.h>
#import "RuntimeTest1ViewController.h"
#import "SUTRuntimeMethodViewController.h"

#import "NSObject+ARDKVO.h"

@interface ViewController ()
{
    Person *_p;
}

@end

@implementation ViewController

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];//-> 已经指向 xxx-viewWillAppear 方法的实现
    NSLog(@"---");
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.frame = CGRectMake(15, 75, 200, 50);
    button.layer.cornerRadius = 3;
    button.layer.borderWidth = 0.8;
    button.layer.borderColor = [UIColor brownColor].CGColor;
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button setTitle:@"runtime1" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(jumpRuntimeVC1) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
    
    UIButton *button1 = [UIButton buttonWithType:UIButtonTypeCustom];
    button1.frame = CGRectMake(15, 155, 200, 50);
    button1.layer.cornerRadius = 3;
    button1.layer.borderWidth = 0.8;
    button1.layer.borderColor = [UIColor brownColor].CGColor;
    [button1 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button1 setTitle:@"runtime3" forState:UIControlStateNormal];
    [button1 addTarget:self action:@selector(jumpRuntimeVC3) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button1];
    
    
    //方法交换Demo
    NSURL *url = [NSURL URLWithString:@"www.baidu.com"];
    
    NSLog(@"%@",url);
    
//    [NSURL performSelector:<#(SEL)#>]
    
    //!------ KVO
    
    _p = [[Person alloc] init];
    
    //系统kvo
//    [_p addObserver:self forKeyPath:@"name" options:NSKeyValueObservingOptionNew context:nil];
    
    //自写kvo
    [_p ARD_addObserver:self forKeyPath:@"name" options:NSKeyValueObservingOptionNew context:nil];
    
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context{
    
    NSLog(@"监听到改变");
    NSLog(@"%@",_p.name);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    static int i = 1;
    _p.name = [NSString stringWithFormat:@"姓名%d",i++];
    NSLog(@"touch %@",_p.name);
}

- (void)dealloc{
    [_p removeObserver:self forKeyPath:@"name"];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)jumpRuntimeVC1{
    RuntimeTest1ViewController *runtimeVC = [[RuntimeTest1ViewController alloc] init];
    [self.navigationController pushViewController:runtimeVC animated:YES];
}

- (void)jumpRuntimeVC3{
    SUTRuntimeMethodViewController *runtimeVC = [[SUTRuntimeMethodViewController alloc] init];
    [self.navigationController pushViewController:runtimeVC animated:YES];
}

@end
