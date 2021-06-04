#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

//
// 스레드 검사
//

#include <QMap>
#include <QQueue>
#include <QThread>
#include <functional>
#include "ConfDefined.h"

class CheckThread : public QThread {
    Q_OBJECT
    public:
        CheckThread(QObject *parent = nullptr);
        ~CheckThread();

        // 로그인 패스위드
        void addPassword(QString password) {
            _password = password;
        }

        // 검사객체 추가
        void addCheckList(QList<CheckType> typeList);

    signals:
        // 검사 끝 발송
        void sendFinish();

        // 검사 결과 발송
        void sendCheckResult(CheckType type, CheckLevel level);

    protected:
        // 스레드 실행
        void run();

    private:
        // 숫자
        bool _isContainsNum(QString str, bool &isAll);

        // 문자
        bool _isContainsLetter(QString str, bool &isAll);

        // 특수문자
        bool _isContainsSpecialChar(QString str, bool &isAll);

        // 패스워드 안전도 검사
        void _checkUserInfor();

        // 화면 보호
        void _checkScreenSaver();

        // 공유보안
        void _checkFileSharing();

    private:
        QString _password;                            // 패스워드
        QQueue<CheckType> _checkQueue;                // 검사 줄
};

#endif // CHECKTHREAD_H
