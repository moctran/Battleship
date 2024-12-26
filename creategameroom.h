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
#include "basegamescreen.h"

class CreateGameRoom : public BaseGameScreen {
    Q_OBJECT

public:
    CreateGameRoom(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    QString generateRoomID();     // Function to generate a random Room ID
    void setRoomID(const QString &roomId);
    void displayRoomID();
    void displayPlayers(QString firstPlayerId, QString secondPlayerId);
    struct Player {
        QString id;
        QString username;
    };
    void onSendButtonClicked(const QString &userId);
    void populateOnlinePlayers(); // Function to populate online players list
    void displayOnlinePlayers(std::vector<Player>& players);
    void updateLabels(const QString player1Name, const QString player2Name);
    void sendJoinRoomRequest(const QString &invitedRoomId);
    void refreshPlayerList();
private slots:
    void onStartGameClicked();
    void onBackClicked();
    void onSendInviteClicked();
    void onPlayerChanges(const QByteArray &message);
    void onSetUpRedirect(const QByteArray &message);
    // Handles game invitations
    void onInvitationReceived(const QByteArray &message);

private:
    QLabel *player1Label;
    QLabel *player2Label;
    QPushButton *startGameButton;
    QPushButton *backButton;
    QPushButton *refreshButton;
    QLabel *roomIDLabel;
    QListWidget *onlinePlayersList;
    QVBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;
    QString roomID;
    // New method for handling leave room request
   void sendLeaveRoomRequest(std::function<void(bool)> callback);
};

#endif // CREATEGAMEROOM_H
