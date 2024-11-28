#ifndef JOINGAMEROOM_H
#define JOINGAMEROOM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class JoinGameRoom : public QWidget {
    Q_OBJECT

public:
    JoinGameRoom(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void onJoinRoomClicked();
    void onBackClicked();

private:
    QLineEdit *roomNumberInput;
    QPushButton *joinRoomButton;
    QPushButton *backButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
};

#endif // JOINGAMEROOM_H
