#include "signupscreen.h"
#include <QDebug> // For logging

SignUpScreen::SignUpScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter username");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);

    signUpButton = new QPushButton("Sign up", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Sign Up Screen", this));
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(signUpButton);
    setLayout(layout);

    connect(signUpButton, &QPushButton::clicked, this, &SignUpScreen::onSignUpButtonClicked);
}

void SignUpScreen::onSignUpButtonClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // // Log the username and password to the console
    // qDebug() << "Sign-up attempt:";
    // qDebug() << "Username:" << username;
    // qDebug() << "Password:" << password;

    // Navigate back to the initial screen (optional)
    stackedWidget->setCurrentIndex(0);
    // Clear input fields
    usernameInput->clear();
    passwordInput->clear();
}
