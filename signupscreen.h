#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class SignUpScreen : public QWidget {
    Q_OBJECT

public:
    SignUpScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onSignUpButtonClicked();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *signUpButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // SIGNUPSCREEN_H
