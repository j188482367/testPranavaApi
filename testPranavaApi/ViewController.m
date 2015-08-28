//
//  ViewController.m
//  testPranavaApi
//
//  Created by 刘俊宏 on 15/8/26.
//  Copyright (c) 2015年 刘俊宏. All rights reserved.
//

#import "ViewController.h"
#import "JsonEncoder.h"
#import "utils.h"
#import "httphelper.h"

@interface ViewController ()
@property(nonatomic,strong) UITextField*serialNumText;
@end

@implementation ViewController

- (void)viewDidLoad {
    _serialNumText=[[UITextField alloc]initWithFrame:CGRectMake(0, 66, self.view.bounds.size.width,20 )];
    _serialNumText.backgroundColor=[UIColor grayColor];
    [self.view addSubview:_serialNumText];
    
    UIButton*generateButton =[[UIButton alloc]initWithFrame:CGRectMake(0,66+_serialNumText.bounds.size.height,self.view.bounds.size.width,20)];
    
    
    
    
    
    [generateButton setTitle:@"generate" forState:UIControlStateNormal];
    
    [generateButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    
    //   [generateButton setTitleColor:[UIColor grayColor] forState:UIControlStateHighlighted];
    
    [generateButton setTitleColor:[UIColor whiteColor] forState:UIControlStateHighlighted];
    
    [generateButton addTarget:self action:@selector(jihuo) forControlEvents:UIControlEventTouchUpInside];
    
    [generateButton.layer setBorderWidth:1.0];
    
    [generateButton.layer setBorderColor:[[UIColor grayColor]CGColor]];
    
    [self.view addSubview:generateButton];
    

    
    
    
    [super viewDidLoad];

    // Do any additional setup after loading the view, typically from a nib.
}




-(void)jihuo
{
    
    
    NSString*k=@"55dde67cbf913";
    
     
    NSDictionary*v=@{@"u":@"fojgbdbghghdhdh",@"a":@"55dd3fe7d00d9",@"v":@[]};
    
    NSError * error;
    
    //json_encode
    
    NSData *toJsonData = [NSJSONSerialization dataWithJSONObject:v options:NSJSONReadingMutableLeaves error:&error];
    
    NSString * jsonString = [[NSString alloc] initWithData:toJsonData encoding:NSUTF8StringEncoding];
    
    
       //@"{\"u\":\"fojgbdbghghdhdh\",\"a\":\"55dd3fe7d00d9\",\"v\":[]}";
    
    NSLog(@"json:%@\r\n",jsonString);
  
    //encrypt
    
    char* str=pr_encrypt([jsonString UTF8String], [k UTF8String] );
    
    
 

    NSString *nssStr= [[NSString alloc] initWithCString:(const char*)str
                                               encoding:NSASCIIStringEncoding];
    
    NSLog(@"encrypt:%@\r\n",nssStr);
        
    NSDictionary*json=@{@"k":k,@"v":nssStr};
  
    
    
  
    
     
     //    NSData --> NSDictionary
     // NSDictionary --> NSData
     NSData *jsonData = [NSJSONSerialization dataWithJSONObject:json options:NSJSONReadingMutableLeaves error:nil];
     
     
     NSString * jsonString2 = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
     
     
     
     
    NSLog(@"json2:%@\r\n",jsonString2);
    
    NSURL *url =[NSURL URLWithString:@"http://www.pranava.cn/auth"];
    
    NSMutableURLRequest*request=[[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:5];
    
    
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody:jsonData];
    
    //创建一个新的队列（开启新线程）
    NSData*received=[NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    
    NSString*receiverDataToString=[[ NSString alloc] initWithData:received encoding:NSUTF8StringEncoding];


    NSLog(@"%@",receiverDataToString);
 
     
    
    
}




- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
