#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <basescreen.h>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTcpSocket>

class LoginScreen : public baseScreen {
    Q_OBJECT

public:
    explicit LoginScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onLoginButtonClicked();
    void onBackButtonClicked();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *backButton; // Declare the back button here
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // LOGINSCREEN_H
