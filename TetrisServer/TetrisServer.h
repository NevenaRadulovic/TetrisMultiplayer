#ifndef TETRISSERVER_H
#define TETRISSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class TetrisServer : public QTcpServer
{
    Q_OBJECT

public:
    TetrisServer(QObject *parent=0);
private slots:
    void readyRead();
    void disconnected();
protected:
    void incomingConnection(qintptr socketID);

private:
    int broj_igraca;
    int maxIgraca;
    int redniBrIgraca;
    int rezultatIgrac1;
    int rezultatIgrac2;
    QString napraviString(int rezultat, int redniBrIgraca);
    void posaljiString(QString poruka1, QString poruka);
    QMap<int, QTcpSocket*> clients;
};

#endif // TETRISSERVER_H

