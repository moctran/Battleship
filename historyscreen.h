#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include "basescreen.h"
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class HistoryScreen : public baseScreen {
    Q_OBJECT

public:
    explicit HistoryScreen(QStackedWidget *stackedWidget, const QString &token = "", QWidget *parent = nullptr);
    void setToken(const QString &newToken);
    void loadHistory();

private slots:
    void onBackButtonClicked();

private:
    void fetchHistory();
    void displayHistory(const QJsonObject &data);

    QStackedWidget *stackedWidget;
    QString token;
    QTableWidget *historyTable;
    QPushButton *backButton;
    QVBoxLayout *layout;
};

#endif // HISTORYSCREEN_H
