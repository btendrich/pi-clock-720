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

    connect(&ntpProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readNtpOutput()));
    connect(&pingProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readPingOutput()));

    textColor = "000000";
    bgColor = "FF0000";
    ui->timeField->setStyleSheet("background: #" + bgColor + "; color: #" + textColor + "; border: 0px ;");
    ui->indicator->setStyleSheet("background: #000000; color: #FF0000; border: 0px ;");
    ui->indicator->setText("Starting Up...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimerTimeout()
{
    ui->timeField->setText( QTime::currentTime().toString("hh:mm:ss AP") );
    ui->timeField->setStyleSheet("background: #" + bgColor + "; color: #" + textColor + "; border: 0px ;");
}

void MainWindow::updateNtpStatusTimeout()
{
    ntpProcess.kill();
    ntpProcess.setProgram("/Users/btendrich/foo.sh");
    ntpProcess.setProgram("/usr/bin/ntpstat");
    ntpProcess.start();

    pingProcess.kill();
    pingProcess.setProgram("/bin/ping");
    pingProcess.setArguments(QStringList() << "-c" << "1" << "-W" << "10" << "8.8.8.8" );
    pingProcess.start();
}

// slot connected to QProcess::finished
void MainWindow::readNtpOutput()
{
    QString string = ntpProcess.readAllStandardOutput();
    QStringList lines = string.split(QRegExp("\n|\r\n|\r"));

    if( lines.at(0).contains("synchronised to NTP server")) {
//        qDebug() << "Syncronized to NTP server.";
        bgColor = "000000";
        textColor = "FF0000";
        if( lines.count() > 1 ) {
            if( lines.at(1).contains("time correct to within")) {
                QStringList args = lines.at(1).trimmed().split(" ");
                int offset = args.at(4).toInt();
                if( offset < 1000 ) {
                    textColor = "FFFF00";
                }
                if( offset < 500 ) {
                    textColor = "00FF00";
                }
//                qDebug() << "Reported offset is " << offset << " ms";

            }
        }
    } else if( lines.at(0).contains("synchronised to unspecified at stratum")) {
//        qDebug() << "Not syncronized to NTP server! Freewheeling";
        bgColor = "FFFF00";
        textColor = "000000";
    } else if( lines.at(0).contains("unsynchronised")) {
//        qDebug() << "Never syncronized to NTP server!";
        bgColor = "FF0000";
        textColor = "000000";
    } else {
        qDebug() << "Unknown state: ";
        qDebug() << lines;
        bgColor = "FFFFFF";
        textColor = "000000";
    }

}

void MainWindow::readPingOutput()
{
    QString string = pingProcess.readAllStandardOutput();
//    qDebug() << "Ping returned: " << string;
    if( string.contains( " 0 received," ) ) {
        ui->indicator->setText("NO INTERNET CONNECTION");
    } else if( string.contains( " 1 received," ) ) {
        ui->indicator->setText("");
    } else {
        ui->indicator->setText("Error");
        qDebug() << "Unknown state: ";
        qDebug() << string;
    }
}
