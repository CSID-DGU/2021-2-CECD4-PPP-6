#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

//
// Component
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

        // 제목
        void addTitle(QString title);

        // 보안 상태
        void addLevel(CheckLevel level);

        // 조치방법
        void addMethod(QString method);
        void setShowMethod(bool isShow);
        void clearTip();

    private slots:
        void _onBtnClicked();

    private:
        bool _isUp;
        bool _isShow;
        QGroupBox *_group;
        QLabel *_levelLabel;
        QLabel *_methodLabel;
        QPushButton *_titleBtn;
};

#endif // COMPONENTWIDGET_H
