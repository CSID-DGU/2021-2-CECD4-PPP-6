#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//
// 主界面
//

#include <QTimer>
#include <QWidget>
#include "CheckThread.h"
#include "TableWidget.h"
#include "CheckThread.h"

class QLabel;
class QGroupBox;
class QPushButton;
class ComponentWidget;
class MainWindow : public QWidget
{
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        // 添加用户密码
        void addPassword(QString password) {
            _password = password;
        }

    private slots:
        // 响应开始按键
        void _onStartBtnClicked();

        // 响应检测结束
        void _onCheckFinish();

        // 响应信号
        void _onCheckSignal(CheckType type, CheckLevel level);

        // 响应定时器事件
        void _onTimerOut();

    private:
        // 初始化样式
        void _initSytle();

        // 初始化检测项
        void _initCheck();

    private:
        unsigned int _count;
        QString _password;
        QString _userModifyInfo;
        QString _screenModifyInfo;
        QString _fileSharingInfo;
        QString _MicrophoneInfo;
        QLabel *_lineLabel;
        QLabel *_tipLineLabel;
        QLabel *_timeLabel;
        QLabel *_totalLabel;
        QLabel *_titleLabel;
        QLabel *_countTimeLabel;

        QPushButton *_startButton;

        QTimer *_timer;
        TableWidget *_table;
        ComponentWidget *_userCompone;
        ComponentWidget *_screenCompone;
        ComponentWidget *_fileCompone;
        ComponentWidget *_microphone;

        CheckThread *_thread;
        QMap<CheckType, QString> _checkMap;
};
#endif // MAINWINDOW_H
