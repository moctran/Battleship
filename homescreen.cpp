#include "homescreen.h"
#include <QMessageBox>

HomeScreen::HomeScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    // Create buttons
    joinGameRoomButton = new QPushButton("Join Game Room", this);
    createGameRoomButton = new QPushButton("Create Game Room", this);
    leaderboardButton = new QPushButton("Leaderboard", this);
    historyButton = new QPushButton("History", this);
    logOutButton = new QPushButton("Log Out", this);

    // Create layout and add buttons
    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Home Screen", this));
    layout->addWidget(joinGameRoomButton);
    layout->addWidget(createGameRoomButton);
    layout->addWidget(leaderboardButton);
    layout->addWidget(historyButton);
    layout->addWidget(logOutButton);
    setLayout(layout);

    // Connect buttons to their respective slots
    connect(joinGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onJoinGameRoomClicked);
    connect(createGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onCreateGameRoomClicked);
    connect(leaderboardButton, &QPushButton::clicked, this, &HomeScreen::onLeaderboardClicked);
    connect(historyButton, &QPushButton::clicked, this, &HomeScreen::onHistoryClicked);
    connect(logOutButton, &QPushButton::clicked, this, &HomeScreen::onLogOutClicked);
}

void HomeScreen::onJoinGameRoomClicked() {
    QMessageBox::information(this, "Join Game Room", "Feature not implemented yet.");
}

void HomeScreen::onCreateGameRoomClicked() {
    QMessageBox::information(this, "Create Game Room", "Feature not implemented yet.");
}

void HomeScreen::onLeaderboardClicked() {
    QMessageBox::information(this, "Leaderboard", "Feature not implemented yet.");
}

void HomeScreen::onHistoryClicked() {
    QMessageBox::information(this, "History", "Feature not implemented yet.");
}

void HomeScreen::onLogOutClicked() {
    stackedWidget->setCurrentIndex(0); // Navigate back to the initial screen
}
