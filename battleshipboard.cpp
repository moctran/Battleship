#include "battleshipboard.h"
#include <QMessageBox>
#include <QDrag>
#include <QDebug>

BattleshipBoard::BattleshipBoard(QStackedWidget *stackedWidget, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidget) {
    setAcceptDrops(true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Player's Board
    playerLayout = new QGridLayout();
    createBoard();
    QVBoxLayout *playerContainer = new QVBoxLayout();
    QLabel *playerLabel = new QLabel("Player's Board", this);
    playerLabel->setAlignment(Qt::AlignCenter);
    playerContainer->addWidget(playerLabel);
    playerContainer->addLayout(playerLayout);

    // Center layout for ships panel and buttons
    QVBoxLayout *centerLayout = new QVBoxLayout();
    gameTitle = new QLabel("Battleship Game", this);
    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    centerLayout->addWidget(gameTitle);

    setupShipsPanel(centerLayout);

    // Finish Setup Button
    finishSetupButton = new QPushButton("Finished Set Up", this);
    finishSetupButton->setFixedSize(150, 40);
    finishSetupButton->setStyleSheet("font-size: 14px;");
    connect(finishSetupButton, &QPushButton::clicked, this, &BattleshipBoard::onFinishSetupClicked);
    centerLayout->addWidget(finishSetupButton, 0, Qt::AlignCenter);

    // Return Button
    returnButton = new QPushButton("Return", this);
    returnButton->setFixedSize(100, 40);
    connect(returnButton, &QPushButton::clicked, this, &BattleshipBoard::onReturnButtonClicked);
    centerLayout->addWidget(returnButton, 0, Qt::AlignCenter);

    // Add to main layout
    mainLayout->addLayout(playerContainer);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(centerLayout);
    setLayout(mainLayout);
}

void BattleshipBoard::createBoard() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            playerBoard[row][col] = new QPushButton();
            playerBoard[row][col]->setFixedSize(40, 40);
            playerBoard[row][col]->setStyleSheet("background-color: lightblue;");
            playerBoard[row][col]->setProperty("state", 0); // Default state = 0
            playerBoard[row][col]->setAcceptDrops(true);
            playerLayout->addWidget(playerBoard[row][col], row, col);
        }
    }
}

void BattleshipBoard::setupShipsPanel(QVBoxLayout *centralPanel) {
    QLabel *shipsLabel = new QLabel("Available Ships", this);
    shipsLabel->setAlignment(Qt::AlignCenter);
    centralPanel->addWidget(shipsLabel);

    QList<int> shipSizes = {2, 2, 3, 3, 4, 5};

    QHBoxLayout *shipsLayout = new QHBoxLayout();
    for (int size : shipSizes) {
        QPushButton *shipButton = new QPushButton(QString::number(size));
        shipButton->setFixedSize(size * 40, 40);
        shipButton->setStyleSheet("background-color: black; color: white; font-weight: bold;");
        shipButton->setProperty("shipSize", size);

        connect(shipButton, &QPushButton::pressed, [=]() {
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(QString::number(size));
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->exec(Qt::MoveAction);
        });

        availableShips.append(shipButton);
        shipsLayout->addWidget(shipButton);
    }
    centralPanel->addLayout(shipsLayout);
}

void BattleshipBoard::dragEnterEvent(QDragEnterEvent *event) {
    //To implement
    qDebug() << "Dragging the ship";
}

void BattleshipBoard::dropEvent(QDropEvent *event) {
    //To implement
    qDebug() << "Drop the ship";
}


bool BattleshipBoard::canPlaceShip(int row, int col, int size) {
    for (int i = 0; i < size; ++i) {
        if (col + i >= 8 || playerBoard[row][col + i]->property("state").toInt() != 0)
            return false;
    }
    return true;
}

void BattleshipBoard::placeShip(int row, int col, int size) {
    for (int i = 0; i < size; ++i) {
        playerBoard[row][col + i]->setStyleSheet("background-color: black;");
        playerBoard[row][col + i]->setProperty("state", 1);
    }
}

void BattleshipBoard::onFinishSetupClicked() {
    setupFinished = true;
    printBoardState();
    finishSetupButton->setDisabled(true);
}

void BattleshipBoard::printBoardState() {
    qDebug() << "Final Board State:";
    for (int row = 0; row < 8; ++row) {
        QString line;
        for (int col = 0; col < 8; ++col) {
            line += QString::number(playerBoard[row][col]->property("state").toInt()) + " ";
        }
        qDebug() << line;
    }
}

void BattleshipBoard::onReturnButtonClicked() {
    QMessageBox::information(this, "Leave Game", "Leaving game room...");
    stackedWidget->setCurrentIndex(3);
}
