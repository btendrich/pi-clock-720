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

    textColor = "FF0000";
    bgColor = "000000";
    ui->timeField->setStyleSheet("background: #" + bgColor + "; color: #" + textColor + "; border: 0px ;");
    ui->indicator->setStyleSheet("background: #" + bgColor + "; color: #" + textColor + "; border: 0px ;");

    ui->indicator->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimerTimeout()
{
    ui->timeField->setText( QTime::currentTime().toString("hh:mm:ss AP") );
}
