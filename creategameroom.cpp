#include "creategameroom.h"
#include <QRandomGenerator>
#include <QMessageBox>

CreateGameRoom::CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    // Left side (Player Info and Buttons)
    player1Label = new QLabel("My Name", this);
    player2Label = new QLabel("Opponent Name (hardcoded)", this);
    startGameButton = new QPushButton("Start Game", this);
    backButton = new QPushButton("Back", this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("Players", this));
    leftLayout->addWidget(player1Label);
    leftLayout->addWidget(new QLabel("VS", this));
    leftLayout->addWidget(player2Label);
    leftLayout->addWidget(startGameButton);
    leftLayout->addWidget(backButton);

    // Right side (Room ID and Online Players)
    roomIDLabel = new QLabel("Room ID: " + generateRoomID(), this);
    onlinePlayersList = new QListWidget(this);

    populateOnlinePlayers();

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(roomIDLabel);
    rightLayout->addWidget(new QLabel("Online Players", this));
    rightLayout->addWidget(onlinePlayersList);

    // Combine left and right side
    QHBoxLayout *screenLayout = new QHBoxLayout();
    screenLayout->addLayout(leftLayout);
    screenLayout->addLayout(rightLayout);

    setLayout(screenLayout);

    // Connect buttons
    connect(startGameButton, &QPushButton::clicked, this, &CreateGameRoom::onStartGameClicked);
    connect(backButton, &QPushButton::clicked, this, &CreateGameRoom::onBackClicked);
}

void CreateGameRoom::onStartGameClicked() {
    QMessageBox::information(this, "Start Game", "Starting the game...");
    // Add actual game logic here
}

void CreateGameRoom::onBackClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}

void CreateGameRoom::onSendInviteClicked() {
    QMessageBox::information(this, "Send Invite", "Invite sent to the selected player!");
}

void CreateGameRoom::populateOnlinePlayers() {
    // Clear any existing items in the list
    onlinePlayersList->clear();

    // Hardcoded list of online players
    QStringList players = {"Player1", "Player2", "Player3", "Player4"};

    for (const QString &player : players) {
        // Create a custom widget for each player
        QWidget *itemWidget = new QWidget(onlinePlayersList);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        // Create the player name label
        QLabel *playerLabel = new QLabel(player, itemWidget);

        // Set a minimum height for the label to ensure text is not cut off
        playerLabel->setMinimumHeight(30);

        // Create the "Send" button
        QPushButton *sendButton = new QPushButton("Send Request", itemWidget);

        // Adjust the button size
        sendButton->setMinimumHeight(30);
        sendButton->setFixedWidth(100);

        // Add the label and button to the layout
        itemLayout->addWidget(playerLabel);
        itemLayout->addWidget(sendButton);
        itemLayout->setContentsMargins(5, 5, 5, 5); // Add some padding
        itemWidget->setLayout(itemLayout);

        // Add the custom widget to the QListWidget
        QListWidgetItem *listItem = new QListWidgetItem(onlinePlayersList);

        // Set a fixed height for the list item to match the widget
        listItem->setSizeHint(QSize(0, 40)); // Set height to 40 pixels

        onlinePlayersList->addItem(listItem);
        onlinePlayersList->setItemWidget(listItem, itemWidget);
    }
}

QString CreateGameRoom::generateRoomID() {
    // Generate a random 6-digit Room ID
    return QString::number(QRandomGenerator::global()->bounded(100000, 999999));
}
