#include "initialscreen.h"



InitialScreen::InitialScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    // Instruction Label
    QLabel *instructions = new QLabel(
        "Welcome to Battleship!\n"
        "1. Log in or sign up to start playing.\n"
        "2. Join an existing room or create a new one to challenge another online player.\n"
        "3. Arrange your ships strategically and take turns hitting opponent's ships.\n"
        "4. View your game history and leaderboard to track your progress."
        , this);
    instructions->setWordWrap(true);
    instructions->setAlignment(Qt::AlignCenter);

    // Buttons for Login and Sign Up
    loginButton = new QPushButton("Log in", this);
    signUpButton = new QPushButton("Sign up", this);

    // Layout for Buttons (Side by Side)
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(signUpButton);

    // Main Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(instructions);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    // Window properties
    setWindowTitle("Initial Screen");
    resize(300, 200);

    // Connect signals to slots
    connect(loginButton, &QPushButton::clicked, this, &InitialScreen::onLoginClicked);
    connect(signUpButton, &QPushButton::clicked, this, &InitialScreen::onSignUpClicked);
}

void InitialScreen::onLoginClicked() {
    stackedWidget->setCurrentIndex(1); // Navigate to Login Screen
}

void InitialScreen::onSignUpClicked() {
    stackedWidget->setCurrentIndex(2); // Navigate to Sign Up Screen
}
