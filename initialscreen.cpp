#include "initialscreen.h"

InitialScreen::InitialScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    loginButton = new QPushButton("Log in", this);
    signUpButton = new QPushButton("Sign up", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(loginButton);
    layout->addWidget(signUpButton);
    setLayout(layout);

    setWindowTitle("Initial Screen");
    resize(300, 200);

    connect(loginButton, &QPushButton::clicked, this, &InitialScreen::onLoginClicked);
    connect(signUpButton, &QPushButton::clicked, this, &InitialScreen::onSignUpClicked);
}

void InitialScreen::onLoginClicked() {
    stackedWidget->setCurrentIndex(1); // Navigate to Login Screen
}

void InitialScreen::onSignUpClicked() {
    stackedWidget->setCurrentIndex(2); // Navigate to Sign Up Screen
}
