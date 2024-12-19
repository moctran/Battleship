#ifndef JOINGAMEROOM_H
#define JOINGAMEROOM_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JoinGameRoom : public QWidget {
    Q_OBJECT

public:
    JoinGameRoom(QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    void setToken(const QString &newToken); // Set token dynamically

private slots:
    void onJoinRoomClicked();
    void onBackClicked();

private:
    QLineEdit *roomNumberInput;
    QPushButton *joinRoomButton;
    QPushButton *backButton;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget;
    QString token;
    void redirect(QString firstPlayerId, QString secondPlayerId, QString roomID);
};

#endif // JOINGAMEROOM_H
