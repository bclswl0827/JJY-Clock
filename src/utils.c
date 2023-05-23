#include "utils.h"

// 实现天数转月份
uint8_t Days2Month(uint8_t days, uint8_t leap) {
    uint8_t rate = days * 1000 / 365;
    if (rate <= 31000 / 365) {
        return 1;
    } else if (rate <= (leap ? 60000 : 59000) / 365) {
        return 2;
    } else if (rate <= (leap ? 91000 : 90000) / 365) {
        return 3;
    } else if (rate <= (leap ? 121000 : 120000) / 365) {
        return 4;
    } else if (rate <= (leap ? 152000 : 151000) / 365) {
        return 5;
    } else if (rate <= (leap ? 182000 : 181000) / 365) {
        return 6;
    } else if (rate <= (leap ? 213000 : 212000) / 365) {
        return 7;
    } else if (rate <= (leap ? 244000 : 243000) / 365) {
        return 8;
    } else if (rate <= (leap ? 274000 : 273000) / 365) {
        return 9;
    } else if (rate <= (leap ? 305000 : 304000) / 365) {
        return 10;
    } else if (rate <= (leap ? 335000 : 334000) / 365) {
        return 11;
    }
    return 12;
}

// 实现天数转日期
uint8_t Days2Date(uint8_t days, uint8_t leap) {
    switch (Days2Month(days, leap)) {
        case 1:
            return 31 - (31 - days);
        case 2:
            return (leap ? 29 : 28) - ((leap ? 60 : 59) - days);
        case 3:
            return 31 - ((leap ? 91 : 90) - days);
        case 4:
            return 30 - ((leap ? 121 : 120) - days);
        case 5:
            return 31 - ((leap ? 152 : 151) - days);
        case 6:
            return 30 - ((leap ? 182 : 181) - days);
        case 7:
            return 31 - ((leap ? 213 : 212) - days);
        case 8:
            return 31 - ((leap ? 244 : 243) - days);
        case 9:
            return 30 - ((leap ? 274 : 273) - days);
        case 10:
            return 31 - ((leap ? 305 : 304) - days);
        case 11:
            return 30 - ((leap ? 335 : 334) - days);
        default:
            return 31 - ((leap ? 366 : 28) - days);
    }
}