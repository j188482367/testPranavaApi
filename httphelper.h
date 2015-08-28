//
//  httphelper.h
//  testPranavaApi
//
//  Created by 刘俊宏 on 15/8/28.
//  Copyright (c) 2015年 刘俊宏. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
//#import "Reachability.h"

@interface HttpHelper : NSObject


+ (NSData*)post:(NSString *)Url RequestParams:(NSDictionary *)params;
@end

