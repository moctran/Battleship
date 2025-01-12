// BaseGameScreen.h
#ifndef BASEGAMESCREEN_H
#define BASEGAMESCREEN_H

#include <QWidget>
#include <QStackedWidget>
#include <basescreen.h>

class BaseGameScreen : public baseScreen {
    Q_OBJECT

public:
    explicit BaseGameScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    void setFirstPlayerStatus(bool check);

protected:
    void leaveRoom(const QString &token, QStackedWidget *stackedWidget);
    bool isFirstPlayer;
    QString getColour(int index);
};

#endif // BASEGAMESCREEN_H
