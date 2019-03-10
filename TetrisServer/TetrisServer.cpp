#include "TetrisServer.h"
#include "mainwindow.h"

TetrisServer::TetrisServer(QObject* parent) : QTcpServer(parent)
{
    broj_igraca=0;
    maxIgraca = 2;
    redniBrIgraca = 1;
    rezultatIgrac1 = 300;
    rezultatIgrac2 = 300;
}
int socketIDs[2];

/*Ova fja se poziva kada se pojavi nova konekcija, socketID je deskriptor soketa za prihvacenu konekciju
Posle taj deskriptor dodeljujemo soketu*/
void TetrisServer::incomingConnection(qintptr socketID)
{
    if(broj_igraca<maxIgraca){
        socketIDs[broj_igraca]=socketID;
        QTcpSocket *client = new QTcpSocket(this);
        client->setSocketDescriptor(socketID);
        clients.insert(socketID, client);
        //qDebug()<<"socketId "<<socketID;
        //qDebug()<<"client "<<client;
        QString redniBrIgracaStr = QString::number(redniBrIgraca);
        client->write(QString(redniBrIgracaStr).toUtf8());
//        client->write(redniBrIgracaStr.toUtf8());

        qDebug() << "New client from:" << client->peerAddress().toString();
        this->redniBrIgraca++;

        connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
        broj_igraca++;
    }
    else{
        qWarning()<<"Maksimalan broj igraca!";
    }
}

QString TetrisServer::napraviString(int rezultat, int redniBrIgraca)
{
    // clientID je prvi u nizu karaktera
    QString poruka;
    QString ostatak;
    if(rezultat <= 0){
        return poruka = "10000";
    }
    else if(rezultat<1000){
            QString nula = "0";
            poruka = QString::number(redniBrIgraca);
            ostatak = nula.append(QString::number(rezultat));
            poruka.append(ostatak);
            return poruka;
    }
    else{
        poruka = QString::number(redniBrIgraca);
        ostatak = QString::number(rezultat);
        poruka.append(ostatak);
        return poruka;
    }
}

//void TetrisServer::posaljiString(QString poruka1, QString poruka2)
//{

//    QString rezultat;

//    rezultat = TetrisServer::napraviString(rezultatIgrac1, 0);
//    QString rest = TetrisServer::napraviString(rezultatIgrac2, 1);
//    rezultat.append(rest);


//    foreach(QTcpSocket *client, clients)
//        client->write(QString(rezultat).toUtf8());

//}


int inicijalnaPrvog = 0;
int inicijalnaDrugog = 0;

void TetrisServer::readyRead()
{
    //qDebug() << "ReadyRead";

    // Koji klijent je poslao poruku

    QTcpSocket *client = (QTcpSocket*)sender();

    QString message = QString::fromUtf8(client->readLine()).trimmed();
    //qDebug()<<message;

    //qDebug() << message << " od: " << client->socketDescriptor();

    QString poruka;
    if(message=="Reset"){
        this->rezultatIgrac1=300;
        this->rezultatIgrac2=300;


    }
    else{

    if (client->socketDescriptor()==socketIDs[0])
    {
        if(inicijalnaPrvog == 0){
            inicijalnaPrvog++;
        }
        else{
            rezultatIgrac1-=100;
            rezultatIgrac2+=100;
            if(rezultatIgrac1==0){
                QString poruka1 = TetrisServer::napraviString(rezultatIgrac1,1);
                QString poruka2 = TetrisServer::napraviString(rezultatIgrac2,1);
                poruka = poruka1.append(poruka2);
            }
            else{
                QString poruka1 = TetrisServer::napraviString(rezultatIgrac1,0);
                QString poruka2 = TetrisServer::napraviString(rezultatIgrac2,0);
                poruka = poruka1.append(poruka2);
            }
            qDebug()<<"poruka od prvog igraca"<<poruka;
            foreach (QTcpSocket *client, clients)//svakom od klijenata se salje poruka
                client->write(QString(poruka).toUtf8());
        }
    }
    else
    {
        if(inicijalnaDrugog == 0){
            inicijalnaDrugog++;
        }
        else{
            rezultatIgrac1+=100;
            rezultatIgrac2-=100;
            if(rezultatIgrac2==0){
                QString poruka1 = TetrisServer::napraviString(rezultatIgrac1,1);
                QString poruka2 = TetrisServer::napraviString(rezultatIgrac2,1);
                poruka = poruka1.append(poruka2);
            }
            else{
                QString poruka1 = TetrisServer::napraviString(rezultatIgrac1,0);
                QString poruka2 = TetrisServer::napraviString(rezultatIgrac2,0);
                poruka = poruka1.append(poruka2);
            }

            qDebug()<<"poruka od drugog igraca"<<poruka;
            // Odgovor ide na oba klijenta
            foreach (QTcpSocket *client, clients)
                client->write(QString(poruka).toUtf8());
        }
   }
  }
}

void TetrisServer::disconnected()
{
    qDebug() << "Disconnected";
}
