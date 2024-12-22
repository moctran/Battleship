#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <basescreen.h>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>

class SignUpScreen : public baseScreen {
    Q_OBJECT

public:
    explicit SignUpScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onSignUpButtonClicked();
    void onBackButtonClicked();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *signUpButton;
    QPushButton *backButton; // Declare the back button here
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // SIGNUPSCREEN_H
