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
    QPushButton *playerBoard[8][8];
    QPushButton *opponentBoard[8][8];
    QPushButton *returnButton;
    QLabel *gameTitle;
    QStackedWidget *stackedWidget;
    QString token; // Player's token will be stored here
    QList<QList<int>> initialBoardState; // Saved board state

    void createBoard(QGridLayout *layout, QPushButton *board[8][8], const QString &boardName);
    void onReturnClicked();
};

#endif // GAMEBOARD_H
