#ifndef INITIALSCREEN_H
#define INITIALSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class InitialScreen : public QWidget {
    Q_OBJECT

public:
    InitialScreen(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onSignUpClicked();

private:
    QPushButton *loginButton;
    QPushButton *signUpButton;
    QVBoxLayout *layout;
};

#endif // INITIALSCREEN_H
