#include "joingameroom.h"
#include <QMessageBox>

JoinGameRoom::JoinGameRoom(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {

    roomNumberInput = new QLineEdit(this);
    roomNumberInput->setPlaceholderText("Enter Room Number");

    joinRoomButton = new QPushButton("Join Room", this);
    backButton = new QPushButton("Back", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Join Game Room", this));
    layout->addWidget(roomNumberInput);
    layout->addWidget(joinRoomButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(joinRoomButton, &QPushButton::clicked, this, &JoinGameRoom::onJoinRoomClicked);
    connect(backButton, &QPushButton::clicked, this, &JoinGameRoom::onBackClicked);
}

void JoinGameRoom::onJoinRoomClicked() {
    QString roomNumber = roomNumberInput->text();
    if (!roomNumber.isEmpty()) {
        QMessageBox::information(this, "Join Game Room", "Joined room: " + roomNumber);
    } else {
        QMessageBox::warning(this, "Join Game Room", "Please enter a room number.");
    }

    // Clear input field after submission
    roomNumberInput->clear();
}

void JoinGameRoom::onBackClicked() {
    stackedWidget->setCurrentIndex(3); // Navigate back to Home Screen
}
