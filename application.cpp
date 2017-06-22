#include "application.h"
#include <QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTime>
#include <QDebug>

Application::Application(QWidget *parent)
    : QWidget(parent),
      m_menu(nullptr),
      m_questions({"Une petite pause ?"}),
      m_timerWorkingMSEC(0)
{
    m_trayIcon = new QSystemTrayIcon(this);

    QMenu *m_menu = new QMenu(this);
    m_menu->addAction(tr("Redémarrer"), this, &Application::startDefaultTimer);
    m_menu->addAction(tr("Configurer"), this, &Application::configure);
    m_menu->addAction(tr("À propos"), this, &Application::about);
    m_menu->addAction(tr("Quitter"), this, &Application::quit);

    m_trayIcon->setContextMenu(m_menu);

    m_timerWorking = new QTimer(this);
    m_timerWorking->setSingleShot(true);
    connect(m_timerWorking, &QTimer::timeout, this, &Application::ask);

    m_timerUpdate = new QTimer(this);
    m_timerUpdate->start(5000);
    connect(m_timerUpdate, &QTimer::timeout, this, &Application::updateIcon);

    m_config = new Config(this);
    connect(m_config, &Config::configChanged, this, &Application::startDefaultTimer);

    startDefaultTimer();
    updateIcon();
}

void Application::run()
{
    m_trayIcon->show();
}

void Application::configure()
{
    m_config->show();
    show();
}

void Application::about()
{
    QMessageBox::about(this, "À propos", "Fait pas super Loïc");
}

void Application::quit()
{
    qApp->quit();
}

void Application::ask()
{
    auto res = QMessageBox::question(this, "Temps écoulé", m_questions.at(qrand()%m_questions.size()));
    if (res == QMessageBox::Ok)
        runApp();
    else
        startTimer(m_config->reportTime());
}

void Application::runApp()
{
    m_timerWorking->stop();
    QDesktopServices::openUrl(m_config->url());
}

void Application::startTimer(const QTime &time)
{
    m_timerWorkingMSEC = time.msecsSinceStartOfDay();
    m_timerWorking->start(m_timerWorkingMSEC);
    updateIcon();
}

void Application::startDefaultTimer()
{
    startTimer(m_config->workingTime());
}

void Application::updateIcon()
{
    double progress = 1;
    if (m_timerWorking->isActive())
        progress = (m_timerWorkingMSEC - m_timerWorking->remainingTime()) / double(m_timerWorkingMSEC);

    int image = int(progress * 5);
    if (image == 5)
        image = 4;

    m_trayIcon->setIcon(QIcon(QString(":/images/cerv%1.png").arg(image)));
    QTime remaining = QTime::fromMSecsSinceStartOfDay(m_timerWorking->remainingTime());
    m_trayIcon->setToolTip(QString(tr("Temps de travail restant : %1:%2").arg(remaining.hour(), remaining.second())));
}
