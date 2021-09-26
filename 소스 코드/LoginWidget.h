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
        // 鼠标移动事件
        void mouseMoveEvent(QMouseEvent *e);

        // 鼠标按下事件
        void mousePressEvent(QMouseEvent *e);

        // 鼠标释放事件
        void mouseReleaseEvent(QMouseEvent *e);

    private slots:
        // 响应登陆按键
        void _onLoginBtnClicked();

    private:
        // 初始化样式
        void _initStyle();

        // 初始化字体
        void _initFont();

        // QString 转wchar_t
        const wchar_t *_stringToWChar(QString str);

    private:
        bool _isPressed;                          // 是否按键
        QPoint _movePoint;                        // 窗体移动坐标点
        QPushButton *_loginBtn;                   // 登陆按键
        QPushButton *_closeBtn;                   // 关闭按键
        QLineEdit *_usernameEdit;                 // 用户名编辑框
        QLineEdit *_passwordEdit;                 // 密码编辑框

        MainWindow *_mainWindow;                  // 主窗体
};

#endif // LOGINWIDGET_H
