#include "config.h"
#include "ui_config.h"
#include <QLineEdit>
#include <QTimeEdit>
#include <QVariant>
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>

Config::Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Config),
    m_settings("CerveauPause", "Lalaband")
{
    ui->setupUi(this);
    connect(ui->cancelButton, &QPushButton::clicked, this, &Config::close);
    connect(ui->saveButton, &QPushButton::clicked, this, [this]() {
        saveSettings();
        close();
    });
    connect(ui->buttonFile, &QPushButton::clicked, this, [this]() {
        ui->urlLineEdit->setText(QFileDialog::getOpenFileName(this));
    });
}

Config::~Config()
{
    delete ui;
}

QUrl Config::url() const
{
    return m_settings.value("url", QUrl()).toUrl();
}

QTime Config::workingTime() const
{
    return m_settings.value("working", QTime(0, 30)).toTime();
}

QTime Config::reportTime() const
{
    return m_settings.value("report", QTime(0, 5)).toTime();
}

void Config::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    loadSettings();
}

void Config::close()
{
    QWidget::close();
    if (parentWidget())
        parentWidget()->close();
}

void Config::loadSettings()
{
    ui->urlLineEdit->setText(url().toString());
    ui->workingTimeEdit->setTime(workingTime());
    ui->reportTimeEdit->setTime(reportTime());
}

void Config::saveSettings()
{
    m_settings.setValue("url", QUrl(ui->urlLineEdit->text()));
    m_settings.setValue("working", ui->workingTimeEdit->time());
    m_settings.setValue("report", ui->reportTimeEdit->time());

    emit configChanged();
}
