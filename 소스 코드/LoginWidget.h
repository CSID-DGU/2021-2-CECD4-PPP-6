#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

//
// 登陆界面
//

#include <QObject>
#include <QWidget>

class QLabel;
class QLineEdit;
class MainWindow;
class QPushButton;
class LoginWidget : public QWidget {
    Q_OBJECT
    public:
        LoginWidget(QWidget *parent = nullptr);
        ~LoginWidget();

    protected:

        void mouseMoveEvent(QMouseEvent *e);

        void mousePressEvent(QMouseEvent *e);

        void mouseReleaseEvent(QMouseEvent *e);

    private slots:
        void _onLoginBtnClicked();

    private:
        void _initStyle();
        void _initFont();
        const wchar_t *_stringToWChar(QString str);

    private:
        bool _isPressed;
        QPoint _movePoint;
        QPushButton *_loginBtn;
        QPushButton *_closeBtn;
        QLineEdit *_usernameEdit;
        QLineEdit *_passwordEdit;

        MainWindow *_mainWindow;
};

#endif // LOGINWIDGET_H
