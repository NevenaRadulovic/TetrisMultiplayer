#ifndef LOGIKA_H
#define LOGIKA_H
#include <QString>
#include <element.h>
#include <mainwindow.h>


class Logika{
private:
    void postaviElement(Element&, int, int);
public:
    Logika(MainWindow*);
    void inicijalizujElement();

    QString Elementi;
    int tabla[20][10];
    Element trenutniElement;
    int rotacijaElementa;
    int izgledElementa;
    bool kraj;
    int rezultat;
    void padanjeElementa();
    bool mozeJosDole();
    void blokirajElement();
    void ucitajPonovoElement();
    bool pada;
    bool mozeJosLevo();
    bool mozeJosDesno();
    bool mozeRotacija(int rotacijaElementa);
    void krajIgre();
    void ciscenjeReda();
    bool cisti;
};

#endif // LOGIKA_H
