#include <QFile>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDateTime>

#include "mainwindow.h"
#include "ComponentWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , _count(0)
    , _lineLabel(new QLabel(this))
    , _tipLineLabel(new QLabel(this))
    , _timeLabel(new QLabel(this))
    , _totalLabel(new QLabel("최종 평점 :   0",this))
    , _titleLabel(new QLabel("개인 PC 보안 상태 점검 및 얼굴 특징 분석을 통한 마이크 사용여부 감시",this))
    , _countTimeLabel(new QLabel(this))
    , _startButton(new QPushButton("점검 시작", this))
    , _timer(new QTimer(this))
    , _table(new TableWidget(this))
    , _userCompone(new ComponentWidget(this))
    , _screenCompone(new ComponentWidget(this))
    , _fileCompone(new ComponentWidget(this))
    , _microphone(new ComponentWidget(this))
    , _thread(new CheckThread())

{
    resize(1000, 800);
    setWindowIcon(QIcon("./Icon/tool.png"));

    _initSytle();

    setWindowTitle("PPP");

    _startButton->setFixedHeight(150);

    qRegisterMetaType<CheckType>("CheckType");
    qRegisterMetaType<CheckLevel>("CheckLevel");

    connect(_timer, SIGNAL(timeout()), this, SLOT(_onTimerOut()));
    connect(_startButton, SIGNAL(clicked(bool)), this, SLOT(_onStartBtnClicked()));
    connect(_thread, SIGNAL(sendFinish()), this, SLOT(_onCheckFinish()), Qt::QueuedConnection);
    connect(_thread, SIGNAL(sendCheckResult(CheckType,CheckLevel)),
            this, SLOT(_onCheckSignal(CheckType,CheckLevel)), Qt::QueuedConnection);

    _lineLabel->setFixedHeight(2);
    _lineLabel->setStyleSheet("QLabel{background-color : gray;}");

    _tipLineLabel->setFixedHeight(2);
    _tipLineLabel->setStyleSheet("QLabel{background-color : gray;}");

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(_titleLabel);

    QGridLayout *timeLayout = new QGridLayout();
    timeLayout->setMargin(2);
    timeLayout->setSpacing(10);
    timeLayout->addWidget(new QLabel("점검 시간 : "), 0, 0, 1, 1, Qt::AlignCenter | Qt::AlignLeft);
    timeLayout->addWidget(_timeLabel, 0, 1, 1, 1, Qt::AlignCenter);
    _countTimeLabel->setVisible(false);

    _totalLabel->setFixedHeight(100);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setMargin(0);
    btnLayout->addWidget(_totalLabel);
    btnLayout->addLayout(timeLayout);
    btnLayout->addWidget(_startButton);

    _userCompone->addTitle("Windows 로그인 패스워드 안전성 여부");
    _screenCompone->addTitle("화면 보호기 설정 여부");
    _fileCompone->addTitle(("사용자 공유폴더 설정 여부"));
    _microphone->addTitle(("마이크 사용여부"));


    QVBoxLayout *comLayout = new QVBoxLayout();
    comLayout->addWidget(_userCompone);
    comLayout->addSpacing(5);
    comLayout->addWidget(_screenCompone);
    comLayout->addSpacing(5);
    comLayout->addWidget(_fileCompone);
    comLayout->addSpacing(5);
    comLayout->addWidget(_microphone);
    comLayout->addStretch(0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(_lineLabel);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(_table);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(_tipLineLabel);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(comLayout);

    setLayout(mainLayout);

    _initCheck();

    QFont font;
    font.setPixelSize(30);
    _totalLabel->setFont(font);
    _totalLabel->setStyleSheet("QLabel{background-color: gary; border-radius: 10px; color : #FFCD42;}");
    _totalLabel->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

    QDateTime date =QDateTime::currentDateTime();
    QString dateStr = date.toString("yyyy-MM-dd hh:mm");
    _timeLabel->setText(dateStr);

    _userModifyInfo = "Start  ->  Setting  ->  Accounts  ->  Sign-in pitions  ->  change";
    _screenModifyInfo = "Right-click property  ->  Personalized  ->  Lock screen  "
                        "->  Screen saver settings";
    _fileSharingInfo = "Right-click myComputer -> management -> shared folder -> shared";

}

MainWindow::~MainWindow()
{
    if (nullptr != _thread) {
        if (_thread->isRunning()) {
            _thread->terminate();
        }

        _thread->deleteLater();
        _thread = nullptr;
    }
}

void MainWindow::_onStartBtnClicked() {
    if ((nullptr == _startButton)
            || (nullptr == _timer)
            || (nullptr == _thread))
    {
        return;
    }

    _startButton->setEnabled(false);

    _count = 0;
    _timer->start(1000);


    if (nullptr == _thread) {
        return;
    }

    _thread->addPassword(_password);
    _thread->addCheckList(_checkMap.keys());
}

void MainWindow::_onCheckFinish() {
    if ((nullptr == _table)
            || (nullptr == _startButton)
            || (nullptr == _timer)
            || (nullptr == _userCompone)
            || (nullptr == _screenCompone))
    {
        return;
    }

    _startButton->setEnabled(true);

    _timer->stop();
    _userCompone->setShowMethod(true);
    _screenCompone->setShowMethod(true);
    _fileCompone->setShowMethod((true));
    _microphone->setShowMethod(true);

    _userCompone->clearTip();
    _screenCompone->clearTip();
    _fileCompone->clearTip();

    int count = 0;
    QMap<CheckType, CheckLevel> checkMap = _table->getCheckState();
    foreach(CheckType type, checkMap.keys()) {
        CheckLevel level = checkMap.value(type);
        switch (level) {
            case CheckLevel::HIGH_LEVEL:
                count += 25;
                break;
            case CheckLevel::MEDIUM_LEVEL:
                count += 15;
                break;
            case CheckLevel::LOW_LEVEL:
                count += 5;
            default:
                break;
        }

        if (CheckLevel::HIGH_LEVEL == level) {
            continue;
        }

        if (type == CheckType::USER_INFO_TYPE) {
            _userCompone->addMethod(_userModifyInfo);
        }
        else if (type == CheckType::SCREEN_SAVER_TYPE) {
            _screenCompone->addMethod(_screenModifyInfo);
        }
        else if (type == CheckType::FILE_SHARING_TYPE) {
            _fileCompone->addMethod(_fileSharingInfo);
        }
    }

    _totalLabel->setText(QString("최종 평점 :   %1").arg(count));
}

void MainWindow::_onCheckSignal(CheckType type, CheckLevel level) {
    if (nullptr == _table) {
        return;
    }

    _table->updateTable(type, level);

    switch(type) {
        case CheckType::USER_INFO_TYPE:
            _userCompone->addLevel(level);
            break;
        case CheckType::SCREEN_SAVER_TYPE:
            _screenCompone->addLevel(level);
            break;
        case CheckType::FILE_SHARING_TYPE:
            _fileCompone->addLevel(level);
            break;
        default:
            break;
    }
}

void MainWindow::_onTimerOut() {
    if (nullptr == _countTimeLabel) {
        return;
    }

    _countTimeLabel->setText(QString("%1").arg(_count++));
}

void MainWindow::_initSytle() {
    QFile file("./flatwhite.qss");
    if (file.open(QFile::ReadOnly)) {
          QString qss = QLatin1String(file.readAll());
          QString paletteColor = qss.mid(20, 7);
          qApp->setPalette(QPalette(QColor(paletteColor)));
          qApp->setStyleSheet(qss);
          file.close();
    }
}

void MainWindow::_initCheck() {
    if (nullptr == _table) {
        return;
    }

    _checkMap.clear();
    _checkMap.insert(CheckType::USER_INFO_TYPE, "Windows 로그인 패스워드 안전성 여부 점검");
    _checkMap.insert(CheckType::SCREEN_SAVER_TYPE, "화면 보호기 설정 여부 점검");
    _checkMap.insert(CheckType::FILE_SHARING_TYPE,"사용자 공유폴더 설정 여부 점검");
    _checkMap.insert(CheckType::MICROPHONE_TYPE,"마이크 사용여부");


    QList<QString> headList;
    headList << "점검 항목" << "결과";

    _table->addHeadList(headList);
    _table->addCheckList(_checkMap);
}
