#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class LoginScreen : public QWidget {
    Q_OBJECT

public:
    LoginScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onLoginButtonClicked();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // LOGINSCREEN_H
