#include <QDir>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QFontDatabase>

#include <windows.h>
#include "MainWindow.h"
#include "LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , _isPressed(false)
    , _loginBtn(new QPushButton("로그인",this))
    , _closeBtn(new QPushButton("X",this))
    , _usernameEdit(new QLineEdit(this))
    , _passwordEdit(new QLineEdit(this))
    , _mainWindow(nullptr)
{

    const int width = 500;
    const int height = 380;
    setFixedSize(width, height);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    setWindowIcon(QIcon("./Icon/tool.png"));

    _initFont();

    QLabel *background = new QLabel(this);
    background->setGeometry(0, 0, width, height);
    background->setStyleSheet("QLabel{background-color:qlineargradient(spread:pad, " \
                              "x1:0, y1: 0, x2:0, y2: 1, stop:0 #CCDAE7, stop: 0.5 #E6EAED,"
                              "stop:1 #E3E8EB); border: 2px solid rgba(44, 52, 54, 0.6);"
                              "}");

    QLabel *iconLabel = new QLabel(this);
    iconLabel->raise();
    iconLabel->setGeometry(186, 45, 128, 128);
    iconLabel->setStyleSheet("QLabel{background-image:url(Icon/User1.ico);}");

    _usernameEdit->setPlaceholderText("Username");
    _passwordEdit->setPlaceholderText("Password");
    _passwordEdit->setEchoMode(QLineEdit::Password);

    int closeX = width - 30;
    _closeBtn->raise();
    _usernameEdit->raise();
    _passwordEdit->raise();
    _loginBtn->raise();
    _closeBtn->setGeometry(closeX, 0, 30, 30);
    _usernameEdit->setGeometry(100, 200, 300, 35);
    _passwordEdit->setGeometry(100, 255, 300, 35);
    _loginBtn->setGeometry(190, 315, 120, 30);

    _closeBtn->setStyleSheet("QPushButton{background-color: transparent;}");
    _usernameEdit->setStyleSheet("QLineEdit{border-radius: 16px; border: 2px solid #FFFFFF;}" \
                                 "QLineEdit:focus{border-color: #ea2;}");
    _passwordEdit->setStyleSheet("QLineEdit{border-radius: 16px; border: 2px solid #FFFFFF;}"
                                 "QLineEdit:focus{border-color: #ea2;}");
    _loginBtn->setStyleSheet("QPushButton{border-radius: 15px; border: 1px solid gray;"
                             "background-color: #2c3436; color: #FFFFFF;} " \
                             "QPushButton:hover{background-color: rgba(44, 52, 54, 0.6)}" \
                             "QPushButton:pressed{background-color: rgba(44, 52, 54, 0.9)}");

    connect(_closeBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(_loginBtn, SIGNAL(clicked(bool)), this, SLOT(_onLoginBtnClicked()));
}

LoginWidget::~LoginWidget() {
    if (nullptr != _mainWindow) {
        _mainWindow->deleteLater();
        _mainWindow = nullptr;
    }
}

void LoginWidget::_onLoginBtnClicked() {
    if ((nullptr == _usernameEdit)
            || (nullptr == _passwordEdit))
    {
        return;
    }

    QString username = _usernameEdit->text();
    QString password = _passwordEdit->text();

    if (username.isEmpty() && password.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "아이디와 패스워드를 입력하세요!!");
        return;
    }

    if (username.isEmpty() && !password.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "아이디를 입력하세요!!");
        return;
    }

    if (!username.isEmpty() && password.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "패스워드를 입력하세요!!");
        return;
    }

    HANDLE hUser;
    bool isSuc = LogonUser(_stringToWChar(username), _stringToWChar(QString(".")),
                          _stringToWChar(password),
                          LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hUser);

    DWORD dwErr = GetLastError();
    if (!isSuc && (dwErr != ERROR_ACCOUNT_RESTRICTION)) {
        QMessageBox::warning(nullptr, "Warning", "아이디와 패스워드 일치하지 않습니다!");
        return;
    }

    if (nullptr != _mainWindow) {
        _mainWindow->deleteLater();
        _mainWindow = nullptr;
    }
    _mainWindow = new MainWindow();
    _mainWindow->addPassword(password);
    _mainWindow->show();
    this->hide();
}

void LoginWidget::mouseMoveEvent(QMouseEvent *e) {
    if (_isPressed) {
        QPoint movePos = e->globalPos();
        move(movePos - _movePoint);
    }
    QWidget::mouseMoveEvent(e);
}

void LoginWidget::mousePressEvent(QMouseEvent *e) {
    if (Qt::LeftButton == e->button()) {
        _isPressed = true;
        _movePoint = e->pos();
    }

    QWidget::mousePressEvent(e);
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *e) {
    _isPressed = false;
    QWidget::mouseReleaseEvent(e);
}

void LoginWidget::_initFont() {
    QString path = QCoreApplication::applicationDirPath() + "/SourceHanSansCN-Normal.ttf";
    int fontId = QFontDatabase::addApplicationFont(path);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).first();

    QFont font(fontFamily);
    QApplication::setFont(font);
}
const wchar_t *LoginWidget::_stringToWChar(QString str) {
    return reinterpret_cast<const wchar_t *>(str.utf16());
}
