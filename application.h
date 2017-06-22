#ifndef APPLICATION_H
#define APPLICATION_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include "config.h"

class Application : public QWidget
{
    Q_OBJECT
public:
    explicit Application(QWidget *parent = nullptr);

    void run();

private:
    void configure();
    void about();
    void quit();

    void ask();
    void runApp();

    void startTimer(const QTime &time);
    void startDefaultTimer();
    void updateIcon();

private:
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_menu;
    QStringList m_questions;
    QTimer *m_timerWorking, *m_timerUpdate;
    int m_timerWorkingMSEC;
    Config *m_config;
};

#endif // APPLICATION_H
