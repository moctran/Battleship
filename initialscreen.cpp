#include "initialscreen.h"
#include <QMessageBox>

InitialScreen::InitialScreen(QWidget *parent)
    : QWidget(parent) {

    // Create buttons
    loginButton = new QPushButton("Log in", this);
    signUpButton = new QPushButton("Sign up", this);

    // Create layout and add buttons
    layout = new QVBoxLayout(this);
    layout->addWidget(loginButton);
    layout->addWidget(signUpButton);

    setLayout(layout);

    // Connect signals to slots
    connect(loginButton, &QPushButton::clicked, this, &InitialScreen::onLoginClicked);
    connect(signUpButton, &QPushButton::clicked, this, &InitialScreen::onSignUpClicked);

    // Set window properties
    setWindowTitle("Initial Screen");
    resize(300, 200);
}

void InitialScreen::onLoginClicked() {
    QMessageBox::information(this, "Log in", "Log in button clicked!");
}

void InitialScreen::onSignUpClicked() {
    QMessageBox::information(this, "Sign up", "Sign up button clicked!");
}
