//
//  TyrantdbGameTracker.h
//  TyrantdbGameTracker
//
//  Created by hs on 15/4/19.
//  Copyright (c) 2015年 xindong. All rights reserved.
//

#import <Foundation/Foundation.h>

// 帐户类型
typedef enum {
    TGTTypeAnonymous = 0, // 匿名用户
    TGTTypeRegistered // 注册用户
} TGTUserType;

// 用户性别
typedef enum {
    TGTSexMale = 0, // 男性
    TGTSexFemale, // 女性
    TGTSexUnknown // 性别未知
} TGTUserSex;

@interface TyrantdbGameTracker : NSObject

/**
 * 调用该接口修改数据发送的域名，有特殊需要时调用，调用必须位于初始化之前
 * 域名必须是https://abc.example.com/的格式，不能为空
 */
+ (void)setHost:(NSString *)host;

/**
 * 初始化，尽早调用
 * appId: 注册游戏时获得的APP ID
 * channel: 分包渠道名称，可为空
 * gameVersion: 游戏版本，可为空，为空时，自动获取游戏安装包的版本（Xcode配置中的Version）
 */
+ (void)onStart:(NSString *)appId channel:(NSString *)channel version:(NSString *)gameVersion;

/**
 * 记录一个用户（注意是平台用户，不是游戏角色！！！！），需要保证唯一性
 * userId: 用户的ID（注意是平台用户ID，不是游戏角色ID！！！！），如果是匿名用户，由游戏生成，需要保证不同平台用户的唯一性
 * userType: 用户类型
 * userSex: 用户性别
 * userAge: 用户年龄，年龄未知传递0
 */
+ (void)setUser:(NSString *)userId userType:(TGTUserType)userType userSex:(TGTUserSex)userSex userAge:(NSInteger)userAge userName:(NSString *)userName;

/**
 * 设置用户等级，初次设置时或升级时调用
 * level: 等级
 */
+ (void)setLevel:(NSInteger)level;

/**
 * 设置用户服务器，初次设置或更改服务器的时候调用
 * server: 服务器
 */
+ (void)setServer:(NSString *)server;

/**
 * 发起充值请求时调用
 * orderId: 订单ID，不能为空
 * product: 产品名称，可为空
 * amount: 充值金额（分）
 * currencyType: 货币类型，可为空，参考：人民币 CNY，美元 USD；欧元 EUR
 * virtualCurrencyAmount: 充值获得的虚拟币
 * payment: 支付方式，可为空，如：支付宝
 */
+ (void)onChargeRequest:(NSString *)orderId product:(NSString *)product amount:(NSInteger)amount currencyType:(NSString *)currencyType virtualCurrencyAmount:(NSInteger)virtualCurrencyAmount payment:(NSString *)payment;

/**
 * 充值成功时调用
 * orderId: 订单ID，不能为空，与上一个接口的orderId对应
 */
+ (void)onChargeSuccess:(NSString *)orderId;

/**
 * 充值失败时调用
 * orderId: 订单ID，不能为空，与上一个接口的orderId对应
 * reason: 失败原因，可为空
 */
+ (void)onChargeFail:(NSString *)orderId reason:(NSString *)reason;

/**
 * 当客户端无法跟踪充值请求发起，只能跟踪到充值成功的事件时，调用该接口记录充值信息
 * orderId: 订单ID，可为空
 * product: 产品名称，可为空
 * amount: 充值金额（单位分，即无论什么币种，都需要乘以100）
 * currencyType: 货币类型，可为空，参考：人民币 CNY，美元 USD；欧元 EUR
 * virtualCurrencyAmount: 充值获得的虚拟币
 * payment: 支付方式，可为空，如：支付宝
 */
+ (void)onChargeOnlySuccess:(NSString *)orderId product:(NSString *)product amount:(NSInteger)amount currencyType:(NSString *)currencyType virtualCurrencyAmount:(NSInteger)virtualCurrencyAmount payment:(NSString *)payment;


/**
 * 目前TapDB SDK仅支持单实例模式，如果多次调用onStart方法，只有最初传入的appid生效。调用该函数可以获取生效的信息
 * 返回值：包含appId和channel的NSDictionary，对应的value均可能为nil
 */
+ (NSDictionary *)getStartInfo;


///////////////////第三方SDK额外调用///////////////////

/**
 * 充值成功时调用，实际调用GoogleAdwords的充值统计接口
 * orderId: 订单ID，可为空
 * amount: 充值金额（单位分，即无论什么币种，都需要乘以100）
 * currencyType: 货币类型，可为空，参考：人民币 CNY，美元 USD；欧元 EUR
 * payment: 支付方式，可为空，如：支付宝
 */
+ (void)chargeAnalytics:(NSString *)orderId amount:(NSInteger)amount currencyType:(NSString *)currencyType payment:(NSString *)payment;

/////////////////////////////////////////////////////



///////////已过期接口，新接入的游戏不要调用///////////////

/**
 * 自定义事件
 * eventName: 事件名称
 * properties: 事件属性
 */
+ (void)onEvent:(NSString *)eventName properties:(NSDictionary *)properties;

/**
 * 获赠虚拟币时调用
 * virtualCurrencyAmount: 获赠虚拟币数量
 * reason: 获赠原因，可为空
 */
+ (void)onReward:(NSInteger)virtualCurrencyAmount reason:(NSString *)reason;

/**
 * 开启一个关卡或任务时调用
 * missionId: 关卡或任务标识
 * missionType: 关卡或任务类型
 */
+ (void)onMissionStart:(NSString *)missionId missionType:(NSString *)missionType;

/**
 * 关卡或任务完成时调用
 * missionId: 关卡或任务标识，与上一个接口的missionId对应
 */
+ (void)onMissionFinish:(NSString *)missionId;

/**
 * 关卡或任务失败时调用
 * missionId: 关卡或任务标识，与上一个接口的missionId对应
 * reason: 失败原因，可为空
 */
+ (void)onMissionFail:(NSString *)missionId reason:(NSString *)reason;

/**
 * 当发生游戏内消费时调用
 * item: 购买物品的标识
 * count: 购买的数量
 * cost: 消费使用的虚拟币数量
 */
+ (void)onItemPurchase:(NSString *)item count:(NSInteger)count costVirtualCurrency:(NSInteger)cost;

/**
 * 消耗物品时调用
 * item: 物品的标识
 * count: 消耗的数量
 */
+ (void)onItemUse:(NSString *)item count:(NSInteger)count;

/**
 * 调用后可以自动捕获未知异常，并发送到服务器做分析
 */
+ (void)captureUncaughtException;

/**
 * 调用该接口统计游戏与服务器的连接健康度，登陆时调用一次即可
 * connectMillis: 与服务器的连接时长，单位毫秒
 * success: 与服务器连接是否成功
 * code: 成功或失败代码，不要太长，并且是方便辨识的字符串，比如成功可以发送“success”
 */
+ (void)serverHealth:(NSInteger)connectMillis success:(BOOL)success withCode:(NSString *)code;

/////////////////////////////////////////////////////

@end
