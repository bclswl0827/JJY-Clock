#ifndef __PRIORITY_H__
#define __PRIORITY_H__

// 分钟位权重
enum minute {
    MINUTE_0 = 40,
    MINUTE_1 = 20,
    MINUTE_2 = 10,
    MINUTE_3 = 0,
    MINUTE_4 = 8,
    MINUTE_5 = 4,
    MINUTE_6 = 2,
    MINUTE_7 = 1,
};

// 小时位权重
enum hour {
    HOUR_0 = 20,
    HOUR_1 = 10,
    HOUR_2 = 0,
    HOUR_3 = 8,
    HOUR_4 = 4,
    HOUR_5 = 2,
    HOUR_6 = 1,
};

// 天数位权重
enum day {
    DAY_0 = 200,
    DAY_1 = 100,
    DAY_2 = 0,
    DAY_3 = 80,
    DAY_4 = 40,
    DAY_5 = 20,
    DAY_6 = 10,
    DAY_7 = 8,
    DAY_8 = 4,
    DAY_9 = 2,
    DAY_10 = 1,
};

// 年份位权重
enum year {
    YEAR_0 = 80,
    YEAR_1 = 40,
    YEAR_2 = 20,
    YEAR_3 = 10,
    YEAR_4 = 8,
    YEAR_5 = 4,
    YEAR_6 = 2,
    YEAR_7 = 1,
};

// 星期位权重
enum week {
    WEEK_0 = 4,
    WEEK_1 = 2,
    WEEK_2 = 1,
};

#endif