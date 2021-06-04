#ifndef CONFDEFINED_H
#define CONFDEFINED_H

#include <QObject>

// 检测类型定义
enum class CheckType {
    USER_INFO_TYPE = 0,           // 계정보안
    SCREEN_SAVER_TYPE = 1,        // 화면보안
    FILE_SHARING_TYPE = 2,        // 공유보안
    MICROPHONE_TYPE = 3,          // 마이크 사용여부
};

// 检测等级定义
enum class CheckLevel {
    HIGH_LEVEL = 0,               // 안전
    MEDIUM_LEVEL = 1,             // 일반
    LOW_LEVEL = 2,                // 위험
};

#endif // CONFDEFINED_H
