#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

//
// 检测线程
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

        // 添加开机密码
        void addPassword(QString password) {
            _password = password;
        }

        // 添加检测对象
        void addCheckList(QList<CheckType> typeList);

    signals:
        // 发送检测结束
        void sendFinish();

        // 发送检测结果
        void sendCheckResult(CheckType type, CheckLevel level);

    protected:
        // 执行线程
        void run();

    private:
        // 是否包含数字，纯数字
        bool _isContainsNum(QString str, bool &isAll);

        // 纯字母
        bool _isContainsLetter(QString str, bool &isAll);

        // 是否包含特殊字符
        bool _isContainsSpecialChar(QString str, bool &isAll);

        // 账户密码安全检测
        void _checkUserInfor();

        // 屏幕保护检测
        void _checkScreenSaver();

        //文件共享检测
        void _checkFileSharing();

    private:
        QString _password;                            // 开机密码
        QQueue<CheckType> _checkQueue;                // 检测队列
};

#endif // CHECKTHREAD_H
