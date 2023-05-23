#ifndef __DEMOD_H__
#define __DEMOD_H__

#include <stdint.h>

// 用于记录讯号携带的数据
struct JJYStatus {
    bool status;             // 是否有数据
    uint16_t time;           // 记录起始时间用于后续计算
    uint16_t previous;       // 记录上一次起始时间
    uint16_t cycle;          // 记录方波周期时间
    uint16_t pulse;          // 记录当前脉冲时间
    uint16_t previousPulse;  // 记录上次脉冲时间
} JJYStatus;

// 用于记录解码器状态
struct DecoderStatus {
    bool status;      // 解码器运行状态
    bool done;        // 解码是否完成
    uint8_t counter;  // 数据缓存位置计数器
    int data[53];     // 除去标记符后的数据缓存
    // 示例数据 https://jjy.nict.go.jp/jjy/trans/timecode1-e.html
    // 分钟位 Maker 后第 0 位起
    // 小时位 P1 后第 3 位起
    // 天数位高 7 位 P2 后第 3 位起
    // 天数位低 4 位 P3 后第 0 位起
    // 年份位 P4 后第 1 位起
    // 星期位 P5 后第 0 位起
    // 0.5s 左右脉冲为 1
    // 0.8s 左右脉冲为 0
    // 0.2s 左右脉冲为标记符
    // 时间资讯为各位数值乘以权重值再求和
} DecoderStatus;

// 用于记录解码出来的时间
struct DateInfo {
    uint8_t second = 0;  // 秒
    uint8_t hour;        // 时
    uint8_t minute;      // 分
    char week[4];        // 星期
    bool forenoon;       // 午前
    uint8_t day;         // 日
    uint8_t month;       // 月
    uint8_t year;        // 年
    bool leap;           // 闰
} DateInfo;

#endif