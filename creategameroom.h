#ifndef CREATEGAMEROOM_H
#define CREATEGAMEROOM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>

class CreateGameRoom : public QWidget {
    Q_OBJECT

public:
    CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onStartGameClicked();
    void onBackClicked();
    void onSendInviteClicked();

private:
    QLabel *player1Label;
    QLabel *player2Label;
    QPushButton *startGameButton;
    QPushButton *backButton;
    QLabel *roomIDLabel;
    QListWidget *onlinePlayersList;
    QVBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;

    void populateOnlinePlayers(); // Function to populate online players list
    QString generateRoomID();     // Function to generate a random Room ID
};

#endif // CREATEGAMEROOM_H
