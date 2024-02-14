#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logika.h"
#include "element.h"
#include<QPainter>
#include<QTimer>
#include<QKeyEvent>
#include<QDebug>
#include<QString>
#include <QMessageBox>

#define Blue "#3C8AD8"
#define Red "#B52B69"
#define Pink "#CD88E2"
#define Green "#3BD955"
#define DarkPink "#A02B9C"
#define Yellow "#E8C91B"
#define Purple "#86269B"

Logika* igra;
QTimer* tajmer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    igra=new Logika(this);
    this->grabKeyboard();//releaseKeyboard()
    tajmer=new QTimer(this);
    connect(tajmer,SIGNAL(timeout()),this,SLOT(tajmer_igre()));
    brzina=500;
    clientId = "";
    socket = new QTcpSocket(this);
    firstConnection = true;

    /*This signal is emitted once every time new data is available for reading from the device's current read channel.
     *  It will only be emitted again once new data is available,
     * such as when a new payload of network data has arrived on your network socket,
     * or when a new block of data has been appended to your device.*/
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //This signal is emitted after connectToHost() has been called and a connection has been successfully established.
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    float pozConnect = (ui->connect->x()+ui->connect->width())/2;
    for(int i=0;i<20;i++) //redovi tabele
        for(int j=0;j<10;j++){ //kolone tabele
            painter.setPen(QPen("#000000"));//pravimo crnu mrezu
            switch(igra->tabla[i][j]){ //elementi koji se blokiraju dobijaju boju u tabeli,to se radi u ovo switch grananju
            case 0:
                painter.setBrush(QBrush("#ffffff"));
                break;
            case 1:
                painter.setBrush(QBrush(Blue));
                break;
            case 2:
                painter.setBrush(QBrush(DarkPink));
                break;
            case 3:
                painter.setBrush(QBrush(Green));
                break;
            case 4:
                painter.setBrush(QBrush(Pink));
                break;
            case 5:
                painter.setBrush(QBrush(Purple));
                break;
            case 6:
                painter.setBrush(QBrush(Red));
                break;
            case 7:
                painter.setBrush(QBrush(Yellow));
                break;
            }
            painter.drawRect(j*30+pozConnect-150,i*30+10,30,30);//crta se kvadrat zadatih dimenzija
        }
    if(tajmer->isActive()){//ako je tajmer aktivan,odnosno vreme tece
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++){
                painter.setPen(QPen("#000000"));

               switch(igra->trenutniElement.elementBoja){//u svakom trenutku bojimo polje tabele,tako simuliramo padanje elementa
               case 0:
                    painter.setBrush(QBrush("#ffffff"));
                   break;
               case 1:
                   painter.setBrush(QBrush(Blue));
                   break;
               case 2:
                   painter.setBrush(QBrush(DarkPink));
                   break;
               case 3:
                   painter.setBrush(QBrush(Green));
                   break;
               case 4:
                   painter.setBrush(QBrush(Pink));
                   break;
               case 5:
                   painter.setBrush(QBrush(Purple));
                   break;
               case 6:
                   painter.setBrush(QBrush(Red));
                   break;
               case 7:
                   painter.setBrush(QBrush(Yellow));
                   break;
               }
               if(igra->trenutniElement.elementMatrica[i][j])
                   painter.drawRect(pozConnect-150+(igra->trenutniElement.elementX+j)*30,10+(igra->trenutniElement.elementY+i)*30,30,30);

            }
      }

}

//Programiranje dogadjaja za kretanje elementa
void MainWindow::keyPressEvent(QKeyEvent *e){
    if(tajmer ->isActive()){ //elemente proveravamo samo ako tajmer radi
        switch (e->key()) {
        case Qt::Key_Left:
            if(!igra->pada)//element pomeramo
                if(igra->mozeJosLevo()) //kada pritisnemo taster za levo treba da proverimo da li uopste moze da ide na tu stranu
                    igra->trenutniElement.elementX--; //ako moze, pomerimo ga
            break;
        case Qt::Key_Right:
            if(!igra->pada)
                if(igra->mozeJosDesno()) //kada pritisnemo taster za desno zelimo da proverimo da li uopste moze da ide na tu stranu
                    igra->trenutniElement.elementX++; //ako moze, pomerimo ga
            break;
        case Qt::Key_Up:
            if(!igra->pada){//proveravamo da li je sledeca rotacija moguca
                if(igra->mozeRotacija(igra->rotacijaElementa < 3 ? igra->rotacijaElementa + 1 : 0)){
                    //ako je moguca rotacija ovde je izaberemo
                    (igra->rotacijaElementa < 3) ? (igra->rotacijaElementa++) : (igra->rotacijaElementa = 0);
                    //pravimo rotirani element
                    igra->ucitajPonovoElement();
                }
            }
            break;
        case Qt::Key_Down:
            if(!igra->pada){ //ako element ne pada
                igra->pada = true; //postavljamo flag da pada
                tajmer->setInterval(50); //ubrzavamo padanje
            }
            break;
    }

    this->repaint();
 }
}

//fja koja kontrolise svaki trenutak
void MainWindow::tajmer_igre(){
    igra->padanjeElementa(); // aktivira padanje elementa
  if(igra->cisti){
  socket->write(QString("Poruka").toUtf8());
  igra->cisti=false;
  }
    this->repaint();
}


void MainWindow::onButtonStartClicked()
{
    if(!tajmer->isActive()) //ako tajmer nije bio aktivan,pokrecemo ga
        tajmer->start(brzina);
}

void MainWindow::onButtonStopClicked()
{
    tajmer->stop();//zaustavljamo tajmer
    igra=new Logika(this);//pravimo novu igru
    brzina=500;//brzina se vraca na staro
    ui->rezultat->display(300);
    this->repaint();//sve se crta ponovo
    socket->write(QString("Reset").toUtf8());//saljemo poruku serveru
}

//resetovanje brzine, ako smo prethodno ubrzali padanje, ovo vraca brzinu na staro za sledeci element
void MainWindow::resetujBrzinu(){
    tajmer->setInterval(brzina);
}

//sta se desava kada se oznaci kraj igre
void MainWindow::krajIgre(){
    tajmer->stop();
    onButtonStopClicked();
}

bool pobedioPrvi = false;
bool pobedioDrugi = false;
QString c;
int broj_poruke = 0;

//readyRead
void MainWindow::readyRead() {
    if(firstConnection) {
        this->clientId = QString::fromUtf8(socket->readLine()).trimmed();
        c = clientId.at(0);
        if(c=="1")
            ui->Igrac->setText("Prvi igrac");
        else if(c=="2")
            ui->Igrac->setText("Drugi igrac");
        firstConnection = false;
    } else {
        QString poruka = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug()<<poruka;
        //qDebug()<<"igra->rezultat "<<igra->rezultat;
        QString rezultatPrvog = poruka.mid(1,4);
        QString rezultatDrugog = poruka.mid(6,4);
        if(c=="1"){
            qDebug()<<poruka;
            ui->rezultat->display(rezultatPrvog.toInt());
            if(rezultatDrugog=="0000"){
                krajIgre();
                QMessageBox::information(this,"Rezultat","Pobeda drugog");

            }
            if(rezultatPrvog=="0000"){
                krajIgre();
                QMessageBox::information(this,"Rezultat","Pobeda");

            }
            qDebug()<<"Prvi LCD "<<rezultatPrvog;
            qDebug()<<"Drugi LCD "<<rezultatDrugog;
        }
        else if(c=="2"){
            ui->rezultat->display(rezultatDrugog.toInt());
            if(rezultatPrvog=="0000"){
                krajIgre();
                QMessageBox::information(this,"Rezultat","Pobeda prvog");

            }
            if(rezultatDrugog=="0000"){
                krajIgre();
                QMessageBox::information(this,"Rezultat","Pobeda");

            }
            qDebug()<<"Prvi LCD "<<rezultatPrvog;
            qDebug()<<"Drugi LCD "<<rezultatDrugog;
        }
    }
}
void MainWindow::connected() {
    socket->write(QString::number(igra->rezultat).toUtf8());
}

void MainWindow::onConnectClicked()
{
    socket->connectToHost("localhost", 5000);
    ui->rezultat->display(300);//treba da se vrati na 1000
    qDebug() << "Connect clicked";
}

//void MainWindow::on_saljiButton_clicked()
//{
  //  socket->write(QString("Poruka").toUtf8());
    //socket->write(QString::number(igra->rezultat).toUtf8());
//}
