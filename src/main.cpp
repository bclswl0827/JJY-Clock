#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "demod.h"
#include "priority.h"
#include "utils.h"

// 重写 digitalRead 适应模块电平
int digitalRead(int pin) {
    return analogRead(pin) > 100 ? LOW : HIGH;
}

// 获取日期
void GetDate() {
    // 计算分钟，索引 0-7
    DateInfo.minute = 1 + DecoderStatus.data[0] * MINUTE_0 +
                      DecoderStatus.data[1] * MINUTE_1 +
                      DecoderStatus.data[2] * MINUTE_2 +
                      DecoderStatus.data[3] * MINUTE_3 +
                      DecoderStatus.data[4] * MINUTE_4 +
                      DecoderStatus.data[5] * MINUTE_5 +
                      DecoderStatus.data[6] * MINUTE_6 +
                      DecoderStatus.data[7] * MINUTE_7;
    // 计算小时，索引 10-16
    DateInfo.hour = DecoderStatus.data[10] * HOUR_0 +
                    DecoderStatus.data[11] * HOUR_1 +
                    DecoderStatus.data[12] * HOUR_2 +
                    DecoderStatus.data[13] * HOUR_3 +
                    DecoderStatus.data[14] * HOUR_4 +
                    DecoderStatus.data[15] * HOUR_5 +
                    DecoderStatus.data[16] * HOUR_6;
    // 计算天数，索引 19-29
    DateInfo.day = DecoderStatus.data[19] * DAY_0 +
                   DecoderStatus.data[20] * DAY_1 +
                   DecoderStatus.data[21] * DAY_2 +
                   DecoderStatus.data[22] * DAY_3 +
                   DecoderStatus.data[23] * DAY_4 +
                   DecoderStatus.data[24] * DAY_5 +
                   DecoderStatus.data[25] * DAY_6 +
                   DecoderStatus.data[26] * DAY_7 +
                   DecoderStatus.data[27] * DAY_8 +
                   DecoderStatus.data[28] * DAY_9 +
                   DecoderStatus.data[29] * DAY_10;
    // 计算年份，索引 36-43
    DateInfo.year = DecoderStatus.data[36] * YEAR_0 +
                    DecoderStatus.data[37] * YEAR_1 +
                    DecoderStatus.data[38] * YEAR_2 +
                    DecoderStatus.data[39] * YEAR_3 +
                    DecoderStatus.data[40] * YEAR_4 +
                    DecoderStatus.data[41] * YEAR_5 +
                    DecoderStatus.data[42] * YEAR_6 +
                    DecoderStatus.data[43] * YEAR_7;
    // 计算星期，索引 44-46
    switch (DecoderStatus.data[44] * WEEK_0 +
            DecoderStatus.data[45] * WEEK_1 +
            DecoderStatus.data[46] * WEEK_2) {
        case 1:
            strcpy(DateInfo.week, "Mon");
            break;
        case 2:
            strcpy(DateInfo.week, "Tue");
            break;
        case 3:
            strcpy(DateInfo.week, "Wed");
            break;
        case 4:
            strcpy(DateInfo.week, "Thu");
            break;
        case 5:
            strcpy(DateInfo.week, "Fri");
            break;
        case 6:
            strcpy(DateInfo.week, "Sat");
            break;
        default:
            strcpy(DateInfo.week, "Sun");
            break;
    }
    // 判断午前
    if (DateInfo.hour < 12) {
        DateInfo.forenoon = true;
    } else {
        DateInfo.forenoon = false;
    }
    // 判断闰年
    if (DateInfo.year % 4) {
        DateInfo.leap = false;
    } else {
        DateInfo.leap = true;
    }
    // 计算月份
    DateInfo.month = Days2Month(DateInfo.day, DateInfo.leap);
    // 计算日期
    DateInfo.day = Days2Date(DateInfo.day, DateInfo.leap);
    // 当分钟为 60，小时加 1
    if (DateInfo.minute == 60) {
        ++DateInfo.hour;
        DateInfo.minute = 0;
    }
}

// JJY 解码器
void JJYDecoder() {
    // 若解码器状态标记为 false 则退出
    if (!DecoderStatus.status) {
        Serial.print("当前没有解码任务，不进行解码");
        Serial.print("\r\n");
        return;
    } else {
        // 计算显示进度
        Serial.print(DecoderStatus.counter * 100 / 53);
        Serial.print("% 正在解码中，请稍候");
        Serial.print("\r\n");
    }
    // 将数据推送到数组
    if (DecoderStatus.counter < 53) {
        switch (JJYStatus.pulse / 100) {
            case 4:
            case 5:
                DecoderStatus.data[DecoderStatus.counter++] = 1;
                break;
            case 7:
            case 8:
                DecoderStatus.data[DecoderStatus.counter++] = 0;
            default:
                break;
        }
    } else {
        // 解析解码后的数据
        GetDate();
        // 打印数据
        Serial.print("20");
        Serial.print(DateInfo.year);
        Serial.print("-");
        Serial.print(DateInfo.month);
        Serial.print("-");
        Serial.print(DateInfo.day);
        Serial.print(" ");
        Serial.print(DateInfo.week);
        Serial.print(" ");
        Serial.print(DateInfo.hour);
        Serial.print(":");
        Serial.print(DateInfo.minute);
        Serial.print(":");
        Serial.print(DateInfo.second);
        Serial.print(" JST\r\n");
        // 将本次任务标记为已完成
        DecoderStatus.done = true;
        // 返回提示
        if (DecoderStatus.done) {
            DecoderStatus.counter = 0;
            DecoderStatus.done = !DecoderStatus.done;
            DecoderStatus.status = !DecoderStatus.status;
            Serial.print("本次解码完成，下次解码在 60s 后开始");
            Serial.print("\r\n");
        }
    }
}

void setup() {
    // 启动串口
    Serial.begin(9600);
    // 模块使能
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);
    // 设定为接收 40 kHz
    pinMode(SWITCH_PIN, OUTPUT);
    digitalWrite(SWITCH_PIN, LOW);
}

void loop() {
    // 读取 JJY 数据
    if (digitalRead(DATA_PIN) == HIGH) {
        // 若 JJY 状态为 false 则记录下时间
        if (!JJYStatus.status) {
            // JJY 状态取反
            JJYStatus.status = !JJYStatus.status;
            JJYStatus.time = millis();
        }
    } else {
        // 若 BPC 状态为 true 则计算周期和脉冲
        if (JJYStatus.status) {
            // 计算方波周期时间
            JJYStatus.cycle = JJYStatus.time - JJYStatus.previous;
            // 计算脉冲时间
            JJYStatus.pulse = millis() - JJYStatus.time;
            JJYStatus.previous = JJYStatus.time;
            JJYStatus.status = !JJYStatus.status;
            // 脉冲时间大于 180 且小于 250 则为定位符
            if (JJYStatus.pulse >= 180 && JJYStatus.pulse <= 250 &&
                !DecoderStatus.status) {
                // 定位到分钟起始则打开解码器
                if (!DecoderStatus.counter) {
                    DecoderStatus.counter++;
                } else if (DecoderStatus.counter == 1 &&
                           JJYStatus.pulse - JJYStatus.previousPulse < 50) {
                    Serial.print("成功获取到定位符");
                    Serial.print("\r\n");
                    DecoderStatus.counter = 0;
                    DecoderStatus.status = true;
                } else {
                    // 解码器未运行时计数器置零
                    if (!DecoderStatus.status) {
                        DecoderStatus.counter = 0;
                    }
                }
            }
            // 执行解码函数
            if (JJYStatus.pulse + JJYStatus.cycle > 1000 &&
                JJYStatus.pulse + JJYStatus.cycle < 2000) {
                JJYDecoder();
            }
            // 本次脉冲时间赋值给上次脉冲时间
            JJYStatus.previousPulse = JJYStatus.pulse;
        }
    }
}