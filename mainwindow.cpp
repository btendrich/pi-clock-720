#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL( timeout()), this, SLOT(updateTimerTimeout()));
    updateTimer->start(15);

    ntpStatusTimer = new QTimer(this);
    connect(ntpStatusTimer, SIGNAL( timeout()), this, SLOT(updateNtpStatusTimeout()));
    ntpStatusTimer->start(30000);

    connect(&pingProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readPingOutput()));

    textColor = "FF0000";
    bgColor = "000000";
    ui->timeField->setStyleSheet("background: #" + bgColor + "; color: #" + textColor + "; border: 0px ;");
    ui->indicator->setStyleSheet("background: #000000; color: #FF0000; border: 0px ;");
    ui->indicator->setText("Starting Up...");

    failureCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimerTimeout()
{
    ui->timeField->setText( QTime::currentTime().toString("hh:mm:ss AP") );
}

void MainWindow::updateNtpStatusTimeout()
{
    pingProcess.kill();
    pingProcess.setProgram("/bin/ping");
    pingProcess.setArguments(QStringList() << "-c" << "1" << "-W" << "10" << "8.8.8.8" );
    pingProcess.start();
}


void MainWindow::readPingOutput()
{
    QString string = pingProcess.readAllStandardOutput();
    if( string.contains( " 0 received," ) ) {
        failureCount = failureCount + 1;
    } else if( string.contains( " 1 received," ) ) {
        failureCount = 0;
    } else {
        qDebug() << "Unknown state: ";
        qDebug() << string;
    }

    if( failureCount > 10 ) {
        ui->indicator->setText("NO INTERNET CONNECTION");
    } else {
        ui->indicator->setText("");
    }
}
