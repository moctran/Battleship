// BaseGameScreen.h
#ifndef BASEGAMESCREEN_H
#define BASEGAMESCREEN_H

#include <QWidget>
#include <QStackedWidget>

class BaseGameScreen : public QWidget {
    Q_OBJECT

public:
    explicit BaseGameScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

protected:
    void leaveRoom(const QString &token, QStackedWidget *stackedWidget);
};

#endif // BASEGAMESCREEN_H
