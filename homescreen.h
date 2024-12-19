#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class HomeScreen : public QWidget {
    Q_OBJECT

public:
    HomeScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onJoinGameRoomClicked();
    void onCreateGameRoomClicked();
    void onLeaderboardClicked();
    void onHistoryClicked();
    void onLogOutClicked();
    void HandleLoggedOut();

private:
    QPushButton *joinGameRoomButton;
    QPushButton *createGameRoomButton;
    QPushButton *leaderboardButton;
    QPushButton *historyButton;
    QPushButton *logOutButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // HOMESCREEN_H
