#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>
#include <QSettings>
#include <QUrl>

namespace Ui {
class Config;
}

class Config : public QWidget
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

    QUrl url() const;
    QTime workingTime() const;
    QTime reportTime() const;

signals:
    void configChanged();

protected:
    void showEvent(QShowEvent *event);

private:
    void close();
    void loadSettings();
    void saveSettings();

private:
    Ui::Config *ui;
    QSettings m_settings;
};

#endif // CONFIG_H
