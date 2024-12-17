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
#include <QTcpSocket>

class CreateGameRoom : public QWidget {
    Q_OBJECT

public:
    explicit CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    void setToken(const QString &newToken); // Set token dynamically
    QString generateRoomID();     // Function to generate a random Room ID
    void setRoomID(const QString &roomId);
    void displayRoomID();

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
    QString token;
    QString roomID;
    void populateOnlinePlayers(); // Function to populate online players list
};

#endif // CREATEGAMEROOM_H
