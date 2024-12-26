#ifndef LEADERBOARDSCREEN_H
#define LEADERBOARDSCREEN_H

#include "basescreen.h"
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QJsonArray>

class LeaderboardScreen : public baseScreen {
    Q_OBJECT

public:
    explicit LeaderboardScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    void setToken(const QString &newToken);  // Set the user token
    void loadLeaderboard();                 // Load the leaderboard data from the server

private:
    QTableWidget *leaderboardTable;         // Table to display leaderboard data
    QPushButton *backButton;                // Back button to navigate back
    QVBoxLayout *layout;                    // Layout for arranging widgets
    QStackedWidget *stackedWidget;          // Stacked widget for navigation
    QString token;                          // User token for authentication

    void fetchLeaderboard();                // Fetch leaderboard data from the server
    void displayLeaderboard(const QJsonArray &data); // Populate the table with leaderboard data

private slots:
    void onBackButtonClicked();             // Slot to handle the back button click
};

#endif // LEADERBOARDSCREEN_H
