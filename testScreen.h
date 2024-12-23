#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class TestScreen:public QWidget {
    Q_OBJECT

public:
    TestScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);


private:
    QLabel *label;
    QStackedWidget *stackedWidget;
    void navigateToHome();
};

#endif // TESTSCREEN_H

