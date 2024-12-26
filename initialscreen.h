#ifndef INITIALSCREEN_H
#define INITIALSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include "loginscreen.h"
#include "signupscreen.h"

class InitialScreen : public QWidget {
    Q_OBJECT

public:
    InitialScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onSignUpClicked();

private:
    QPushButton *loginButton;
    QPushButton *signUpButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // INITIALSCREEN_H
