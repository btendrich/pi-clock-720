#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTimerTimeout();
    void updateNtpStatusTimeout();
    void readNtpOutput();
    void readPingOutput();

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;
    QTimer *ntpStatusTimer;
    QProcess ntpProcess;
    QProcess pingProcess;
    QString textColor;
    QString bgColor;
};

#endif // MAINWINDOW_H
