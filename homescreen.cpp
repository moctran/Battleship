#include "homescreen.h"
#include "historyscreen.h"
#include "creategameroom.h"
#include "joingameroom.h"
#include "SocketManager.h"
#include "leaderboardscreen.h"
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QApplication>

extern QString globalUserToken;
extern QString globalUserId;
extern QString globalUserName;

HomeScreen::HomeScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : baseScreen(parent), stackedWidget(stackedWidget) {

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

    // Connect to SocketManager's signal
    SocketManager *socketManager = SocketManager::getInstance();
    connect(socketManager, &SocketManager::messageReceived,
            this, &HomeScreen::onInvitationReceived);
}

void HomeScreen::onJoinGameRoomClicked() {
    JoinGameRoom *joinGameScreen = dynamic_cast<JoinGameRoom *>(stackedWidget->widget(4)); // Index 4
    if (joinGameScreen) {
        joinGameScreen->setToken(globalUserToken); // Pass the token dynamically
        joinGameScreen->setId(globalUserId);
        joinGameScreen->setName(globalUserName);
    }
    stackedWidget->setCurrentIndex(4); // Navigate to Join Game Room Screen
}

void HomeScreen::onCreateGameRoomClicked() {
    CreateGameRoom *createGameRoom = dynamic_cast<CreateGameRoom *>(stackedWidget->widget(5)); // Index 5
    if (createGameRoom) {
        createGameRoom->setToken(globalUserToken); // Pass the token dynamically
        createGameRoom->setId(globalUserId);
        createGameRoom->setName(globalUserName);
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
    LeaderboardScreen *leaderboardScreen = dynamic_cast<LeaderboardScreen *>(stackedWidget->widget(10)); // Index 10
    if (leaderboardScreen) {
        leaderboardScreen->setToken(globalUserToken); // Pass the token dynamically
        leaderboardScreen->loadLeaderboard();        // Trigger fetching leaderboard
    }
    stackedWidget->setCurrentIndex(10); // Navigate to Leaderboard screen
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
    QJsonObject requestJson;
    requestJson["type"] = "logout";
    requestJson["token"] = globalUserToken;

    QByteArray responseData = sendRequest(requestJson, 60000);
    QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = responseDoc.object();

    qDebug() << "Server response for log out:" << responseData;

    if (responseObj["status"].toString() == "success") {
        QMessageBox::information(this, "Logout Successful", "You have logged out from the game.");
        stackedWidget->setCurrentIndex(0); // Navigate back to Initial Screen
    } else {
        QString errorMessage = responseObj["message"].toString();
        QMessageBox::critical(this, "Logout Failed", errorMessage);
    }
}



void HomeScreen::onInvitationReceived(const QByteArray &message) {
    if (stackedWidget->currentWidget() != this) {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(message);
    if (!doc.isObject()) {
        qWarning() << "Invalid message format received.";
        return;
    }

    QJsonObject json = doc.object();
    if (json["type"] != "INVITATION") {
        qWarning() << "Message is not an invitation. Ignoring.";
        return;
    }

    // Extract data from the invitation
    QJsonObject data = json["data"].toObject();
    QString roomId = data["id"].toString();
    QString firstPlayerId = data["firstPlayerId"].toString();
    QString secondPlayerId = data["secondPlayerId"].toString();

    // Create a unique QMessageBox for HomeScreen
    QMessageBox *dialog = new QMessageBox(this);
    dialog->setWindowTitle("Game Invitation");
    dialog->setText(QString("Do you want to join the game room created by Player %1?").arg(firstPlayerId));
    dialog->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dialog->setDefaultButton(QMessageBox::No);

    // Connect dialog finished signal
    QMetaObject::Connection connection;
    connection = connect(dialog, &QMessageBox::finished, this, [this, dialog, roomId, firstPlayerId, secondPlayerId, connection](int result) {
        // Disconnect the dialog's finished signal to ensure no further connections persist
        disconnect(connection);
        dialog->deleteLater(); // Clean up the dialog after it's closed

        if (result == QMessageBox::Yes) {
            if (globalUserToken.isEmpty()) {
                QMessageBox::critical(this, "Error", "Token is not set. Please log in again.");
                return;
            }

            // Prepare the "join_room" request
            QJsonObject joinRequest;
            joinRequest["type"] = "join_room";
            joinRequest["token"] = globalUserToken;
            joinRequest["room_id"] = roomId;

            QJsonDocument joinDoc(joinRequest);
            QByteArray joinData = joinDoc.toJson(QJsonDocument::Compact);

            // Log the request for debugging
            qDebug() << "Sending join_room request: " << joinData;

            // Create a new QTcpSocket for the request
            QTcpSocket *socket = new QTcpSocket(this);
            connect(socket, &QTcpSocket::connected, this, [socket, joinData]() {
                socket->write(joinData);
                socket->flush();
                qDebug() << "Join room request sent.";
            });

            connect(socket, &QTcpSocket::readyRead, this, [socket, this, roomId, firstPlayerId, secondPlayerId]() {
                QByteArray response = socket->readAll();
                socket->close();
                socket->deleteLater();

                // Handle server response
                QJsonDocument responseDoc = QJsonDocument::fromJson(response);
                if (!responseDoc.isObject()) {
                    QMessageBox::critical(this, "Error", "Invalid response received from the server.");
                    return;
                }

                QJsonObject responseObj = responseDoc.object();
                qDebug() << "Server response for join_room: " << response;

                if (responseObj["status"].toString() == "success") {
                    qDebug() << "Successfully joined the room. Navigating to CreateGameRoom screen.";

                    // Navigate to the CreateGameRoom screen
                    CreateGameRoom *createGameRoom = dynamic_cast<CreateGameRoom *>(stackedWidget->widget(5)); // Adjust index if needed
                    if (createGameRoom) {
                        // Pass token and room ID
                        createGameRoom->setToken(globalUserToken);
                        createGameRoom->setRoomID(roomId);

                        // Update labels dynamically (using IDs from the message)
                        createGameRoom->updateLabels(firstPlayerId, secondPlayerId);

                        // Populate online players
                        createGameRoom->populateOnlinePlayers();

                        // Display room ID
                        createGameRoom->displayRoomID();
                    }

                    // Set current index to CreateGameRoom
                    stackedWidget->setCurrentIndex(5);

                } else {
                    QString errorMessage = responseObj["message"].toString();
                    QMessageBox::critical(this, "Error", "Failed to join the room: " + errorMessage);
                }
            });

            connect(socket, &QTcpSocket::errorOccurred, this, [socket]() {
                qWarning() << "Socket error occurred: " << socket->errorString();
                socket->deleteLater();
                QMessageBox::critical(nullptr, "Error", "Failed to connect to the server.");
            });

            // Connect to the server
            socket->connectToHost("127.0.0.1", 8080);
        } else {
            qDebug() << "User declined the invitation.";
        }
    });

    dialog->show();
}
