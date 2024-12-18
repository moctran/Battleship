#include "battleshipboard.h"
#include "gameboard.h"
#include <QMessageBox>
#include <QDrag>
#include <QDebug>

extern QString globalUserToken;

// This is a set up board
// Constructor of a set up board
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

    // Clear Button
    clearButton = new QPushButton("Clear Board", this);
    clearButton->setFixedSize(150, 40);
    connect(clearButton, &QPushButton::clicked, this, &BattleshipBoard::onClearButtonClicked);
    centerLayout->addWidget(clearButton, 0, Qt::AlignCenter);

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

    // Orientation Toggle
    orientationButton = new QPushButton("Horizontal", this);
    orientationButton->setCheckable(true);
    orientationButton->setStyleSheet("font-size: 14px;");
    connect(orientationButton, &QPushButton::clicked, [=]() {
        orientationButton->setText(orientationButton->isChecked() ? "Vertical" : "Horizontal");
    });
    centralPanel->addWidget(orientationButton, 0, Qt::AlignCenter);

    // Add Ships
    QList<int> shipSizes = {5, 4, 3, 2, 2};
    QVBoxLayout *shipsLayout = new QVBoxLayout(); // Use QVBoxLayout instead of QHBoxLayout
    for (int size : shipSizes) {
        QPushButton *shipButton = new QPushButton(QString::number(size));
        shipButton->setFixedSize(size * 40, 40);
        shipButton->setStyleSheet("background-color: black; color: white; font-weight: bold;");
        shipButton->setProperty("shipSize", size);

        connect(shipButton, &QPushButton::pressed, [=]() {
            QPixmap pixmap(shipButton->size());
            shipButton->render(&pixmap); // Render the shipButton's visual to the pixmap

            // Rotate the pixmap if the orientation is vertical
            if (orientationButton->isChecked()) {
                QTransform transform;
                transform.rotate(90); // Rotate 90 degrees
                pixmap = pixmap.transformed(transform, Qt::SmoothTransformation);
            }

            QMimeData *mimeData = new QMimeData();
            mimeData->setText(QString::number(size)); // Ship size

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap); // Set the pixmap for visual feedback

            // Adjust the hotSpot based on orientation
            QPoint hotSpot = QPoint(20, 20);
            drag->setHotSpot(hotSpot);

            if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
                shipButton->setDisabled(true); // Disable button after dragging
                shipButton->setText("Placed");
                shipButton->setStyleSheet("color: white;");
            }
        });

        availableShips.append(shipButton);
        shipsLayout->addWidget(shipButton); // Add buttons to vertical layout
    }
    centralPanel->addLayout(shipsLayout); // Add vertical layout to central panel

    // Add Play Game Button
    playGameButton = new QPushButton("Play Game", this);
    playGameButton->setFixedSize(150, 40);
    playGameButton->setStyleSheet("font-size: 14px;");
    connect(playGameButton, &QPushButton::clicked, this, &BattleshipBoard::onPlayGameClicked);
    centralPanel->addWidget(playGameButton, 0, Qt::AlignCenter);
}

void BattleshipBoard::resetBoardState() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            playerBoard[row][col]->setStyleSheet("background-color: lightblue;");
            playerBoard[row][col]->setProperty("state", 0);
        }
    }
    setupFinished = false;
    finishSetupButton->setEnabled(true);

    // Re-enable all ship buttons
    for (QPushButton *shipButton : availableShips) {
        shipButton->setEnabled(true);
        shipButton->setStyleSheet("background-color: black; color: white; font-weight: bold;");
        shipButton->setText(QString::number(shipButton->property("shipSize").toInt()));
    }
    shipPlace = 0;
}

void BattleshipBoard::onClearButtonClicked() {
    resetBoardState();
}

void BattleshipBoard::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction(); // Allow the drag operation
    }
}

void BattleshipBoard::dropEvent(QDropEvent *event) {
    if (setupFinished) return;

    int shipSize = event->mimeData()->text().toInt(); // Get ship size from MIME data
    QPushButton *targetButton = qobject_cast<QPushButton *>(childAt(event->position().toPoint()));
    if (!targetButton) return;

    int row = -1, col = -1;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (playerBoard[r][c] == targetButton) {
                row = r;
                col = c;
                break;
            }
        }
    }

    if (row == -1 || col == -1) return;

    bool vertical = orientationButton->isChecked();
    if (canPlaceShip(row, col, shipSize, vertical)) {
        placeShip(row, col, shipSize, vertical);
        event->acceptProposedAction(); // Accept drop only on successful placement
    } else {
        QMessageBox::warning(this, "Invalid Placement", "Cannot place ship here!");
    }
}

bool BattleshipBoard::canPlaceShip(int row, int col, int size, bool vertical) {
    for (int i = 0; i < size; ++i) {
        int checkRow = vertical ? row + i : row;
        int checkCol = vertical ? col : col + i;

        if (checkRow >= 8 || checkCol >= 8 || playerBoard[checkRow][checkCol]->property("state").toInt() != 0)
            return false;
    }
    return true;
}

void BattleshipBoard::placeShip(int row, int col, int size, bool vertical) {
    shipPlace = shipPlace + 1;
    for (int i = 0; i < size; ++i) {
        int setRow = vertical ? row + i : row;
        int setCol = vertical ? col : col + i;

        playerBoard[setRow][setCol]->setStyleSheet("background-color: black;");
        switch (shipPlace) {
        case 1:
            playerBoard[setRow][setCol]->setStyleSheet("background-color: red;");
            break;
        case 2:
            playerBoard[setRow][setCol]->setStyleSheet("background-color: orange;");
            break;
        case 3:
            playerBoard[setRow][setCol]->setStyleSheet("background-color: yellow;");
            break;
        case 4:
            playerBoard[setRow][setCol]->setStyleSheet("background-color: green;");
            break;
        case 5:
            playerBoard[setRow][setCol]->setStyleSheet("background-color: blue;");
            break;
        }
        playerBoard[setRow][setCol]->setProperty("state", shipPlace);
    }
}

void BattleshipBoard::onFinishSetupClicked() {
    // Check if any shipButton is still enabled
    for (QPushButton *shipButton : availableShips) {
        if (shipButton->isEnabled()) {
            // Show error message
            QMessageBox::warning(this, "Error", "All ships must be placed before finishing setup!");
            return; // Stop further execution
        }
    }
    // If all ships are placed, proceed
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
    resetBoardState();
    stackedWidget->setCurrentIndex(3);
}

// Add Play Game Handler
void BattleshipBoard::onPlayGameClicked() {
    if (!finishSetupButton->isEnabled()) {
        // Save board state and proceed
        saveBoardState();
        QMessageBox::information(this, "Game Ready", "Setup complete! Starting the game...");
        // For demonstration, we print the saved state
        qDebug() << "Game Board Saved.";
        GameBoard *playgameboard = dynamic_cast<GameBoard *>(stackedWidget->widget(8));
        if (playgameboard) {
            playgameboard->setToken(globalUserToken); // Pass the token dynamically
            playgameboard->setInitialState(savedBoardState);
            playgameboard->displayInitialState();
        }
        stackedWidget->setCurrentIndex(8);
    } else {
        // Show error if setup is incomplete
        QMessageBox::warning(this, "Setup Incomplete", "You must finish placing all ships before starting the game!");
    }
}

// Save the board state
void BattleshipBoard::saveBoardState() {
    savedBoardState.clear(); // Clear any previous saved state
    for (int row = 0; row < 8; ++row) {
        QList<int> rowState;
        for (int col = 0; col < 8; ++col) {
            rowState.append(playerBoard[row][col]->property("state").toInt());
        }
        savedBoardState.append(rowState);
    }

    // Debug output of the saved board state
    qDebug() << "Saved Board State:";
    for (const QList<int> &row : savedBoardState) {
        QString line;
        for (int cell : row) {
            line += QString::number(cell) + " ";
        }
        qDebug() << line;
    }
}

// Set user's token here
void BattleshipBoard::setToken(const QString &newToken) {
    token = newToken;
    qDebug() << "Token set in CreateGameScreen:" << token;
}
