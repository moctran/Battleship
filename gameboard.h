#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDrag>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class GameBoard : public QWidget {
    Q_OBJECT

public:
    GameBoard(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    void setToken(const QString &newToken); // Set token dynamically
    void setInitialState(QList<QList<int>> initialState);
    void displayInitialState();

private:
    QGridLayout *playerLayout;
    QGridLayout *opponentLayout;
    QPushButton *playerBoard[10][10];
    QPushButton *opponentBoard[10][10];
    QPushButton *returnButton;
    QPushButton *makeMoveButton; // Button to confirm the move
    QLabel *gameTitle;
    QStackedWidget *stackedWidget;
    QString token; // Player's token will be stored here
    QList<QList<int>> initialBoardState; // Saved board state
    QPushButton *selectedBlock; // Tracks the currently selected block on the opponent's board

    void createBoard(QGridLayout *layout, QPushButton *board[10][10], const QString &boardName);
    void connectOpponentBoardButtons(); // Connects opponent board buttons to a slot
    void sendMoveRequest(int row, int col);

private slots:
    void onReturnClicked();            // Handles the return button
    void onOpponentBlockClicked();     // Handles a block being clicked on the opponent's board
    void onMakeMoveClicked();          // Handles the Make Move button click
};

#endif // GAMEBOARD_H
