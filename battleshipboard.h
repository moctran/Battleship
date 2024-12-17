#ifndef BATTLESHIPBOARD_H
#define BATTLESHIPBOARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QPair>

class BattleshipBoard : public QWidget {
    Q_OBJECT

public:
    BattleshipBoard(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGridLayout *playerLayout;
    QPushButton *playerBoard[8][8];
    QPushButton *returnButton;
    QPushButton *finishSetupButton;
    QStackedWidget *stackedWidget;
    QLabel *gameTitle;

    QList<QPushButton *> availableShips;
    bool setupFinished = false;
    void createBoard();
    void setupShipsPanel(QVBoxLayout *centralPanel);
    void printBoardState();
    void onReturnButtonClicked();
    void onFinishSetupClicked();
    bool canPlaceShip(int row, int col, int size);
    void placeShip(int row, int col, int size);
};

#endif // BATTLESHIPBOARD_H
