#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTcpSocket>
#include "basescreen.h"

class HistoryScreen : public baseScreen {
    Q_OBJECT

public:
    explicit HistoryScreen(QStackedWidget *stackedWidget, const QString &token, QWidget *parent = nullptr);

    void setToken(const QString &newToken); // Set token dynamically
    void loadHistory();                     // Trigger fetching history

private slots:
    void fetchHistory();
    void onBackButtonClicked();

private:
    QVBoxLayout *layout;
    QLabel *summaryLabel;
    QListWidget *historyList;
    QPushButton *backButton;
    QStackedWidget *stackedWidget;
    QString token;

    void displayHistory(const QJsonObject &data);
};

#endif // HISTORYSCREEN_H
