#ifndef CONFDEFINED_H
#define CONFDEFINED_H

//
// 配置定义项
//
#include <QObject>

// 检测类型定义
enum class CheckType {
    USER_INFO_TYPE = 0,           // 用户信息类型
    SCREEN_SAVER_TYPE = 1,        // 屏保类型
    FILE_SHARING_TYPE = 2,        // 文件共享
    MICROPHONE_TYPE = 3,
};

// 检测等级定义
enum class CheckLevel {
    HIGH_LEVEL = 0,               // 高
    MEDIUM_LEVEL = 1,             // 中
    LOW_LEVEL = 2,                // 低
};

#endif // CONFDEFINED_H
