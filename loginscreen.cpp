#include "loginscreen.h"
#include <QDebug>
#include <QMessageBox>

LoginScreen::LoginScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter username");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Log in", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Login Screen", this));
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
}

void LoginScreen::onLoginButtonClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // Hardcoded sign-in logic
    if (username == "chau" && password == "123") {
        qDebug() << "Login successful!";
        stackedWidget->setCurrentIndex(3); // Navigate to Home Screen
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
    // Clear input fields
    usernameInput->clear();
    passwordInput->clear();
}
