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

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;
    QTimer *ntpStatusTimer;
    QString textColor;
    QString bgColor;
    int failureCount;
};

#endif // MAINWINDOW_H
