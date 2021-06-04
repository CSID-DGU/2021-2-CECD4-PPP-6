#include <QLabel>
#include <QPalette>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include "ComponentWidget.h"

ComponentWidget::ComponentWidget(QWidget *parent)
    : QWidget(parent)
    , _isUp(false)
    , _isShow(false)
    , _group(new QGroupBox(this))
    , _levelLabel(new QLabel(this))
    , _methodLabel(new QLabel(this))
    , _titleBtn(new QPushButton(this))
{

    _titleBtn->setEnabled(false);
    _titleBtn->setFixedHeight(40);
    _titleBtn->setIcon(QIcon("./Icon/down.png"));
    _titleBtn->setLayoutDirection(Qt::LeftToRight);
    _titleBtn->setStyleSheet("QPushButton{background: lightgray; text-align: left;}");
    connect(_titleBtn, SIGNAL(clicked(bool)), this, SLOT(_onBtnClicked()));

    QGridLayout *gridLayout = new QGridLayout();
    QGridLayout *gridLayout1 = new QGridLayout();
    gridLayout->setMargin(5);
    gridLayout->setSpacing(10);
    gridLayout->addWidget(new QLabel("보안 상태 : "), 0, 0, 1, 1, Qt::AlignCenter | Qt::AlignLeft);
    gridLayout->addWidget(_levelLabel, 0, 1, 1, 1, Qt::AlignCenter | Qt::AlignLeft);
    gridLayout->addWidget(new QLabel("조치 방법 : "), 1, 0, 1, 1, Qt::AlignCenter | Qt::AlignLeft);
    gridLayout->addWidget(_methodLabel, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignCenter);

    _group->setLayout(gridLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_titleBtn);
    mainLayout->addWidget(_group);
    setLayout(mainLayout);

    _group->setVisible(false);
}

ComponentWidget::~ComponentWidget() {

}

void ComponentWidget::addTitle(QString title) {
    if (nullptr == _titleBtn) {
        return;
    }

    _titleBtn->setText(title);
}

void ComponentWidget::addLevel(CheckLevel level) {
    if (nullptr == _levelLabel) {
        return;
    }

    QString str = "";
    QString colorStr = "#FFFFFF";
    switch(level) {
        case CheckLevel::HIGH_LEVEL :
            str = "안전";
            colorStr = "#1BA160";
            break;
        case CheckLevel::MEDIUM_LEVEL:
            str = "일반";
            colorStr = "#FFCD42";
            break;
        case CheckLevel::LOW_LEVEL :
            str = "취약";
            colorStr = "#DD5145";
            break;
        default:
            break;
    }

    _levelLabel->setText(str);
    _levelLabel->setStyleSheet(QString("QLabel{color : %1}").arg(colorStr));
}

void ComponentWidget::addMethod(QString method) {
    if (nullptr == _methodLabel) {
        return;
    }

    _methodLabel->setWordWrap(true);
    _methodLabel->setText(method);
}

void ComponentWidget::setShowMethod(bool isShow) {
    _isShow = isShow;
    _titleBtn->setEnabled(isShow);
}

void ComponentWidget::clearTip() {
    if (nullptr == _methodLabel) {
        return;
    }

    _methodLabel->clear();
}

void ComponentWidget::_onBtnClicked() {
    if (nullptr == _titleBtn) {
        return;
    }

    if (_isUp) {
        _isUp = false;
        _titleBtn->setIcon(QIcon("./Icon/down.png"));
    }
    else {
        _isUp = true;
        _titleBtn->setIcon(QIcon("./Icon/Up.png"));
    }

    if (!_isShow) {
        return;
    }

    _group->setVisible(_isUp);
}
