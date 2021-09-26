
#include <windows.h>
#include "CheckThread.h"

/////////////////////////////////////////////////////////////////////////////////////
// 构造函数
/////////////////////////////////////////////////////////////////////////////////////
CheckThread::CheckThread(QObject *parent)
    : QThread(parent)
{

}

/////////////////////////////////////////////////////////////////////////////////////
// 析构函数
/////////////////////////////////////////////////////////////////////////////////////
CheckThread::~CheckThread() {

}

/////////////////////////////////////////////////////////////////////////////////////
// 添加检测队列
/////////////////////////////////////////////////////////////////////////////////////
void CheckThread::addCheckList(QList<CheckType> typeList) {
    if (typeList.isEmpty()) {
        return;
    }

    // 入队
    foreach(CheckType type, typeList) {
        _checkQueue.enqueue(type);
    }

    // 开启线程
    if (!isRunning()) {
        start();
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 线程执行函数
/////////////////////////////////////////////////////////////////////////////////////
void CheckThread::run() {
    while(1) {
        // 退出线程
        if (_checkQueue.isEmpty()) {
            emit sendFinish();
            break;
        }

        // 出队
        CheckType type = _checkQueue.dequeue();
        switch(type) {
            case CheckType::USER_INFO_TYPE :
                _checkUserInfor();
                break;
            case CheckType::SCREEN_SAVER_TYPE :
                _checkScreenSaver();
                break;
            case CheckType::FILE_SHARING_TYPE :
                _checkFileSharing();
                break;
            default:
                break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 账户密码安全检测
/////////////////////////////////////////////////////////////////////////////////////
void CheckThread::_checkUserInfor() {
    // 校验密码长度
    int length = _password.length();
    if (length < 6) {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    // 校验密码组合
    bool isNumAll = false;
    bool isDigital = _isContainsNum(_password, isNumAll);
    if (isDigital && isNumAll) {
        // 纯数字
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    bool isLetterAll = false;
    bool isLetter = _isContainsLetter(_password, isLetterAll);
    if (isLetter && isLetterAll) {
        // 纯字母
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    // 是否包含特殊字符
    bool isSpeicalAll = false;
    bool isSpeical = _isContainsSpecialChar(_password, isSpeicalAll);
    if (isSpeical && isSpeicalAll) {
        // 纯字母
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    if (isSpeical && isDigital && isLetter) {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::HIGH_LEVEL);
    }
    else {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::MEDIUM_LEVEL);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 屏幕保护检测
/////////////////////////////////////////////////////////////////////////////////////
void CheckThread::_checkScreenSaver() {
    // 获取屏幕注册表配置信息
    HKEY screen = nullptr;

    // 打开注册表
    bool isOpen = false;
    long ret = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &screen);
    if (ret == ERROR_SUCCESS) {
        DWORD data = 0;

        // 取值屏保注册信息
        long save = RegQueryValueEx(screen, TEXT("SCRNSAVE.EXE"), NULL, NULL, NULL, &data);
        if (save == ERROR_SUCCESS) {
            isOpen = true;
        }
    }

    // 关闭注册表
    RegCloseKey(screen);
    screen = nullptr;

    // 发送数据结果
    //CheckLevel level = (isOpen) ? CheckLevel::HIGH_LEVEL : CheckLevel::LOW_LEVEL;
    CheckLevel level = (isOpen) ? CheckLevel::HIGH_LEVEL : CheckLevel::LOW_LEVEL;
    emit sendCheckResult(CheckType::SCREEN_SAVER_TYPE, level);
}

/////////////////////////////////////////////////////////////////////////////////////
// 文件安全共享检测
/////////////////////////////////////////////////////////////////////////////////////

void CheckThread::_checkFileSharing() {

    HKEY file = nullptr;
    DWORD dwIndex = 0;

    bool isShare = true;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\ControlSet001\\Services\\LanmanServer\\Shares"), 0, KEY_READ, &file) == ERROR_SUCCESS){
            TCHAR valueName[MAX_PATH] = { 0 }; 
            DWORD length = MAX_PATH;           
            BYTE keyData[MAX_PATH] = { 0 };   
            DWORD lengthData = MAX_PATH;      
            DWORD dwType = REG_SZ;	           
            while (RegEnumValue(file, dwIndex, valueName, &length, 0, &dwType, keyData, &lengthData) == ERROR_SUCCESS){
                dwIndex++;
                length = MAX_PATH;
                lengthData = MAX_PATH;
            }
    }
    if(dwIndex > 0){
        isShare = false;
    }

    RegCloseKey(file);
    file = nullptr;

    CheckLevel level = (isShare) ? CheckLevel::HIGH_LEVEL : CheckLevel::LOW_LEVEL;
    emit sendCheckResult(CheckType::FILE_SHARING_TYPE, level);
}

/////////////////////////////////////////////////////////////////////////////////////
// 是否包含数字，是否纯数字
/////////////////////////////////////////////////////////////////////////////////////
bool CheckThread::_isContainsNum(QString str, bool &isAll) {
    QByteArray byte = str.toLatin1();
    const char *s = byte.data();
    bool isContains = false;
    isAll = true;
    while(*s)
    {
        if((*s>='0' && *s<='9') )
        {
            isContains = true;
        }
        else
        {
            isAll = false;
        }
        s++;
    }
    return isContains;
}

/////////////////////////////////////////////////////////////////////////////////////
// 纯字母
/////////////////////////////////////////////////////////////////////////////////////
bool CheckThread::_isContainsLetter(QString str, bool &isAll) {
    QByteArray byte = str.toLatin1();
    const char *s = byte.data();
    bool isContains = false;
    isAll = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z'))
        {
            isContains = true;
        }
        else
        {
            isAll = false;
        }
        s++;
    }
    return isContains;
}

/////////////////////////////////////////////////////////////////////////////////////
// // 检验是否包含特殊字符
/////////////////////////////////////////////////////////////////////////////////////
bool CheckThread::_isContainsSpecialChar(QString str, bool &isAll)
{
    QByteArray byte = str.toLatin1();
    const char *s = byte.data();
    bool isContains = false;
    isAll = true;
    while(*s)
    {
        if((*s>=' ' && *s<='/') || (*s>=':' && *s<='@')
                || (*s>='[' && *s<='`') || (*s>='{' && *s<='~'))
        {
            isContains = true;
        }
        else
        {
            isAll = false;
        }
        s++;
    }
    return isContains;
}
