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
    void setToken(const QString &newToken); // Set token dynamically

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGridLayout *playerLayout;
    QPushButton *playerBoard[8][8];
    QPushButton *returnButton;
    QPushButton *finishSetupButton;
    QPushButton *clearButton;
    QStackedWidget *stackedWidget;
    QLabel *gameTitle;
    QPushButton *orientationButton;
    QString token; // Player token will be stored here

    QList<QPushButton *> availableShips; // Store all the ship buttons
    QMap<int, QString> shipColors; // Map ship size to color
    bool setupFinished = false;
    QMap<int, int> shipCount; // Track remaining ships per size

    QPushButton *playGameButton;  // Play Game button
    QList<QList<int>> savedBoardState; // Saved board state
    int shipPlace = 0; // Number of ship placed

    void saveBoardState();
    void onPlayGameClicked();
    void createBoard();
    void setupShipsPanel(QVBoxLayout *centralPanel);
    void resetBoardState();
    void printBoardState();
    bool canPlaceShip(int row, int col, int size, bool vertical);
    void placeShip(int row, int col, int size, bool vertical);
    void onReturnButtonClicked();
    void onFinishSetupClicked();
    void onClearButtonClicked();
};

#endif // BATTLESHIPBOARD_H
