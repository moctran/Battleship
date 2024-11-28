#include "homescreen.h"
#include "historyscreen.h"
#include <QDebug>
extern QString globalUserToken;

HomeScreen::HomeScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    joinGameRoomButton = new QPushButton("Join Game Room", this);
    createGameRoomButton = new QPushButton("Create Game Room", this);
    leaderboardButton = new QPushButton("Leaderboard", this);
    historyButton = new QPushButton("History", this);
    logOutButton = new QPushButton("Log Out", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Home Screen", this));
    layout->addWidget(joinGameRoomButton);
    layout->addWidget(createGameRoomButton);
    layout->addWidget(leaderboardButton);
    layout->addWidget(historyButton);
    layout->addWidget(logOutButton);
    setLayout(layout);

    connect(joinGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onJoinGameRoomClicked);
    connect(createGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onCreateGameRoomClicked);
    connect(leaderboardButton, &QPushButton::clicked, this, &HomeScreen::onLeaderboardClicked);
    connect(historyButton, &QPushButton::clicked, this, &HomeScreen::onHistoryClicked);
    connect(logOutButton, &QPushButton::clicked, this, &HomeScreen::onLogOutClicked);
}

void HomeScreen::onJoinGameRoomClicked() {
    stackedWidget->setCurrentIndex(4); // Navigate to Join Game Room Screen
}

void HomeScreen::onCreateGameRoomClicked() {
    stackedWidget->setCurrentIndex(5); // Navigate to Create Game Room Screen
}

void HomeScreen::onLeaderboardClicked() {
    qDebug() << "Leaderboard clicked.";
}

void HomeScreen::onHistoryClicked() {
    HistoryScreen *historyScreen = dynamic_cast<HistoryScreen *>(stackedWidget->widget(6)); // Index 6
    if (historyScreen) {
        historyScreen->setToken(globalUserToken); // Pass the token dynamically
        historyScreen->loadHistory();            // Trigger fetching history
    }
    stackedWidget->setCurrentIndex(6); //
}

void HomeScreen::onLogOutClicked() {
    stackedWidget->setCurrentIndex(0); // Navigate back to Initial Screen
}
