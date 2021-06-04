
#include <windows.h>
#include "CheckThread.h"

CheckThread::CheckThread(QObject *parent)
    : QThread(parent)
{

}

CheckThread::~CheckThread() {

}

void CheckThread::addCheckList(QList<CheckType> typeList) {
    if (typeList.isEmpty()) {
        return;
    }

    foreach(CheckType type, typeList) {
        _checkQueue.enqueue(type);
    }

    if (!isRunning()) {
        start();
    }
}

void CheckThread::run() {
    while(1) {
        if (_checkQueue.isEmpty()) {
            emit sendFinish();
            break;
        }

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

void CheckThread::_checkUserInfor() {
    int length = _password.length();
    if (length < 6) {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    bool isNumAll = false;
    bool isDigital = _isContainsNum(_password, isNumAll);
    if (isDigital && isNumAll) {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    bool isLetterAll = false;
    bool isLetter = _isContainsLetter(_password, isLetterAll);
    if (isLetter && isLetterAll) {
        emit sendCheckResult(CheckType::USER_INFO_TYPE, CheckLevel::LOW_LEVEL);
        return;
    }

    bool isSpeicalAll = false;
    bool isSpeical = _isContainsSpecialChar(_password, isSpeicalAll);
    if (isSpeical && isSpeicalAll) {
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

void CheckThread::_checkScreenSaver() {
    HKEY screen = nullptr;

    bool isOpen = false;
    long ret = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &screen);
    if (ret == ERROR_SUCCESS) {
        DWORD data = 0;

        long save = RegQueryValueEx(screen, TEXT("SCRNSAVE.EXE"), NULL, NULL, NULL, &data);
        if (save == ERROR_SUCCESS) {
            isOpen = true;
        }
    }

    RegCloseKey(screen);
    screen = nullptr;

    CheckLevel level = (isOpen) ? CheckLevel::HIGH_LEVEL : CheckLevel::LOW_LEVEL;
    emit sendCheckResult(CheckType::SCREEN_SAVER_TYPE, level);
}
void CheckThread::_checkFileSharing() {}
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
