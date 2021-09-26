#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

//
// 组件窗体
//

#include <QWidget>
#include "ConfDefined.h"

class QLabel;
class QGroupBox;
class QPushButton;
class ComponentWidget : public QWidget {
    Q_OBJECT
    public:
        ComponentWidget(QWidget *parent = nullptr);
        ~ComponentWidget();

        // 添加标题
        void addTitle(QString title);

        // 添加级别
        void addLevel(CheckLevel level);

        // 添加解决方法
        void addMethod(QString method);

        // 是否显示方法
        void setShowMethod(bool isShow);

        // 清除提示
        void clearTip();

    private slots:
        // 响应按键点击事件
        void _onBtnClicked();

    private:
        bool _isUp;                                 // 是否向上
        bool _isShow;                               // 是否显示方法
        QGroupBox *_group;                          // 组件
        QLabel *_levelLabel;                        // 等级标签
        QLabel *_methodLabel;                       // 解决方法标签
        QPushButton *_titleBtn;                     // 标题按键
};

#endif // COMPONENTWIDGET_H
