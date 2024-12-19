#include "homescreen.h"
#include "historyscreen.h"
#include "creategameroom.h"
#include "joingameroom.h"
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QApplication>

extern QString globalUserToken;

HomeScreen::HomeScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    joinGameRoomButton = new QPushButton("Join Game Room", this);
    createGameRoomButton = new QPushButton("Create Game Room", this);
    leaderboardButton = new QPushButton("Leaderboard", this);
    historyButton = new QPushButton("History", this);
    logOutButton = new QPushButton("Log Out", this);
    testButton = new QPushButton("Test", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Home Screen", this));
    layout->addWidget(joinGameRoomButton);
    layout->addWidget(createGameRoomButton);
    layout->addWidget(leaderboardButton);
    layout->addWidget(historyButton);
    layout->addWidget(logOutButton);
    layout->addWidget(testButton);
    setLayout(layout);

    connect(joinGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onJoinGameRoomClicked);
    connect(createGameRoomButton, &QPushButton::clicked, this, &HomeScreen::onCreateGameRoomClicked);
    connect(leaderboardButton, &QPushButton::clicked, this, &HomeScreen::onLeaderboardClicked);
    connect(historyButton, &QPushButton::clicked, this, &HomeScreen::onHistoryClicked);
    connect(logOutButton, &QPushButton::clicked, this, &HomeScreen::onLogOutClicked);
    connect(testButton, &QPushButton::clicked, this, &HomeScreen::onTestClicked);
}

void HomeScreen::onJoinGameRoomClicked() {
    JoinGameRoom *joinGameScreen = dynamic_cast<JoinGameRoom *>(stackedWidget->widget(4)); // Index 4
    if (joinGameScreen) {
        joinGameScreen->setToken(globalUserToken); // Pass the token dynamically
    }
    stackedWidget->setCurrentIndex(4); // Navigate to Join Game Room Screen
}

void HomeScreen::onCreateGameRoomClicked() {
    CreateGameRoom *createGameRoom = dynamic_cast<CreateGameRoom *>(stackedWidget->widget(5)); // Index 5
    if (createGameRoom) {
        createGameRoom->setToken(globalUserToken); // Pass the token dynamically
        QString roomId = createGameRoom->generateRoomID();

        if (roomId.isEmpty() || roomId == "failed") {
            qDebug() << "Room ID generation failed. Cannot create game room.";
            return; // Stop further processing
        }

        qDebug() << "Generated Room ID: " << roomId;
        createGameRoom->setRoomID(roomId);
        createGameRoom->displayRoomID();
        createGameRoom->populateOnlinePlayers();
        // Navigate to Create Game Room Screen only if RoomID generation succeeds
        stackedWidget->setCurrentIndex(5);
    }
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
    HandleLoggedOut(); // Call logout logic first
}


void HomeScreen::HandleLoggedOut() {
    QTcpSocket socket;
    socket.connectToHost("192.168.10.103", 8080);

    if (!socket.waitForConnected(3000)) {
        QMessageBox::critical(this, "Connection Error", "Failed to connect to the server.");
        return;
    }

    QJsonObject json;
    json["type"] = "logout";
    json["token"] = globalUserToken;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    if (socket.write(data) == -1) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return;
    }

    if (!socket.waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Error", "Failed to send data to the server.");
        return;
    }

    if (!socket.waitForReadyRead(3000)) {
        QMessageBox::critical(this, "Error", "No response from the server.");
        return;
    }

    QByteArray responseData = socket.readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    qDebug() << "Server response for view_profile:" << responseData;

    if (responseObj["status"].toString() == "success") {
        QMessageBox::information(this, "Logout Successful", "You have logged out from the game.");
        stackedWidget->setCurrentIndex(0); // Navigate back to Initial Screen
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Logout Failed", errorMessage);
    }
}

void HomeScreen::onTestClicked() {
    stackedWidget->setCurrentIndex(9); //
}




