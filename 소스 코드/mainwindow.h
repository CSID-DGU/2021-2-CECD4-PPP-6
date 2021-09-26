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
        unsigned int _count;                    // 当前计时
        QString _password;                      // 密码
        QString _userModifyInfo;                // 用户修改提示信息
        QString _screenModifyInfo;              // 屏保修改提示信息
        QString _fileSharingInfo;
        QString _MicrophoneInfo;
        QLabel *_lineLabel;                     // 横线标签
        QLabel *_tipLineLabel;                  // 提示横线标签
        QLabel *_timeLabel;                     // 当前时间标签
        QLabel *_totalLabel;                    // 总分标签
        QLabel *_titleLabel;                    // 标题标签
        QLabel *_countTimeLabel;                // 计时标签

        QPushButton *_startButton;              // 开始按键

        QTimer *_timer;                         // 定时器
        TableWidget *_table;                    // 表格
        ComponentWidget *_userCompone;          // 用户组件
        ComponentWidget *_screenCompone;        // 屏幕组件
        ComponentWidget *_fileCompone;          // 文件共享
        ComponentWidget *_microphone;           // 麦克分使用

        CheckThread *_thread;                   // 检测线程
        QMap<CheckType, QString> _checkMap;     // 检测项映射表
};
#endif // MAINWINDOW_H
