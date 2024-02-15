#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TetrisServer.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new TetrisServer();
    //listen govori serveru na kojoj adresi i na kom portu da ocekuje konekcije
    bool success = server->listen(QHostAddress::Any, 5000);
    if(!success)
    {
        qFatal("Could not listen on port 5000.");
        ui->label->setText("Server nije spreman");
    }
    else ui->label->setText("Server je spreman");
    qDebug() << "Server started";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exit_button_clicked()
{
    //treba odraditi gasenje servera na dugme
    //mislim da je ovo resenje...
    //proveriti
    server->close();//ovako se gasi server!
    MainWindow::close();
}
