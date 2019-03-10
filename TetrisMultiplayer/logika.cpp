#include "logika.h"
#include <iostream>
#include "mainwindow.h"
#include<ctime>
MainWindow *mainApp;

Logika::Logika(MainWindow *kontekst){
    Elementi =
            "1111" // |
            "0000"
            "0000"
            "0000"

            "1000"
            "1000"
            "1000"
            "1000"

            "1111"
            "0000"
            "0000"
            "0000"

            "1000"
            "1000"
            "1000"
            "1000"

            "1000" //obrnuto L
            "1110"
            "0000"
            "0000"

            "1100"
            "1000"
            "1000"
            "0000"

            "1110"
            "0010"
            "0000"
            "0000"

            "0100"
            "0100"
            "1100"
            "0000"

            "0010" // L
            "1110"
            "0000"
            "0000"

            "1000"
            "1000"
            "1100"
            "0000"

            "1110"
            "1000"
            "0000"
            "0000"

            "1100"
            "0100"
            "0100"
            "0000"

            "1100" //kocka
            "1100"
            "0000"
            "0000"

            "1100"
            "1100"
            "0000"
            "0000"

            "1100"
            "1100"
            "0000"
            "0000"

            "1100"
            "1100"
            "0000"
            "0000"

            "0110" // S
            "1100"
            "0000"
            "0000"

            "1000"
            "1100"
            "0100"
            "0000"

            "0110"
            "1100"
            "0000"
            "0000"

            "1000"
            "1100"
            "0100"
            "0000"

            "0100" // obrnuto T
            "1110"
            "0000"
            "0000"

            "1000"
            "1100"
            "1000"
            "0000"

            "1110"
            "0100"
            "0000"
            "0000"

            "0100"
            "1100"
            "0100"
            "0000"

            "1100" // Z
            "0110"
            "0000"
            "0000"

            "0100"
            "1100"
            "1000"
            "0000"

            "1100"
            "0110"
            "0000"
            "0000"

            "0100"
            "1100"
            "1000"
            "0000";

kraj = false; // flag koji oznacava kraj igre - inicijalno je na false
pada = false; // flag koji oznacava da li je u datom trenutku element u fazi padanja
mainApp = kontekst;
rezultat = 1000; // inicijalni rezultat

std::time_t trenutnoVreme = std::time(0);
srand(trenutnoVreme);
cisti=false;
for(int i=0; i<20; i++)
    for(int j=0; j<10; j++) tabla[i][j] = 0;
inicijalizujElement();
}


// fja koja sluzi da napravi jedan element

void Logika::postaviElement(Element &e, int izgled, int rotacija){
   e.elementBoja = izgled+1; // svaki od elemenata ima svoju boju
   e.elementX = 3; // element se stvara od 4 kolone
   e.elementY = 0; // element krece od nultog reda

   int izaberi = izgled * 16 * 4 + rotacija * 16;// ovo je int koji ce da predstavlja indeks elementa u nizu Elementi
   for(int i=0; i<4; i++) // element cini matrica 4x4, mi upisujemo u matricu red po red
       for(int j=0; j<4; j++){
           e.elementMatrica[i][j] = Elementi[izaberi] == '0' ? false:true;// matrica koja predstavlja element dobija svoj izgled na osnovu stringa Elementi
           izaberi++; //indeks u stringu Elementi se inkrementira
       }
}


void Logika::inicijalizujElement(){
    izgledElementa = rand()%7; // random broj 0-6
    rotacijaElementa = rand()%4; // random broj 0-3

    postaviElement(trenutniElement, izgledElementa, rotacijaElementa); // nakon sto odredimo konacan izgled, pravimo element
}

// fja koja izvodi padanje elementa
void Logika::padanjeElementa(){
    if(mozeJosDole() && !kraj){
        trenutniElement.elementY++;
    }
    else{
        blokirajElement();
        krajIgre();
        ciscenjeReda();
        inicijalizujElement();
    }
}


// fja koja proverava da li element moze da ide za jos jednu poziciju na dole
bool Logika::mozeJosDole(){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){

            if(trenutniElement.elementMatrica[i][j] && tabla[trenutniElement.elementY+1+i][trenutniElement.elementX+j]!=0)
                return false;

            if(trenutniElement.elementMatrica[i][j] && trenutniElement.elementY+1+i==20)
                return false;
        }
    return true;
}


// fja blokira element kada vise ne moze da pada
void Logika::blokirajElement(){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            if(trenutniElement.elementMatrica[i][j]) // za svaki od true celija matrice dodeljujemo tabli boju elementa
                tabla[trenutniElement.elementY+i][trenutniElement.elementX+j]=trenutniElement.elementBoja;
        }
    if(pada){
        mainApp->resetujBrzinu();
        pada = false;
    }

}


// kod rotacije elementa je potrebno da ponovo ucitamo element kada mu odredimo rotaciju
void Logika::ucitajPonovoElement(){
    int izaberi = izgledElementa * 16 * 4 + rotacijaElementa * 16;
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            trenutniElement.elementMatrica[i][j] = Elementi[izaberi]=='0' ? false : true;
            izaberi++;
        }
}


// fja koja proverava da li element moze da se pomera u levu stranu
// moze da se pomera ulevo ako nije skroz levo na njoj ili ako se levo od njega ne nalazi neki od elemenata
bool Logika::mozeJosLevo(){
    if(trenutniElement.elementX==0) return false; // ako je uz levu ivicu vracamo false
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            if(trenutniElement.elementMatrica[i][j]){ // za svaku od true celija matrice elementa
              // if(trenutniElement.elementX + j - 1 >= 0) // ?
                    // ako je vec postoji element na tabli levo od tekuceg elementa, fja vraca false
                   if(tabla[trenutniElement.elementY+i][trenutniElement.elementX + j - 1] !=0 )return false;
            }
        }
    return true;
}


// fja koja proverava da li element moze da se pomera u desnu stranu
//  slicna situacija kao i za fju iznad
// razlika je u tome sto ne mozemo da izvrsimo proveru pre for petlje
// jer je potrebno da na koordinate elementa dodamo broj celije elementa, a za to su potrebne for petlje
bool Logika::mozeJosDesno(){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            if(trenutniElement.elementMatrica[i][j]){ // ako je celija matrice trenutnog elementa true
                if(trenutniElement.elementX+j+1==10) return false; // ako je element uz desnu ivicu to je false
                // ili ako se vec nalazi nesto desno od tekuceg elementa
                if(tabla[trenutniElement.elementY+i][trenutniElement.elementX+j+1]!=0) return false;
            }
        }
    return true; // u ostalim slucajevima fja vraca true
}


// fja proverava da li element moze da se rotira
bool Logika::mozeRotacija(int rotacijaElementa){ // moramo da uzmemo u obzir sledecu rotaciju
    int izaberi = izgledElementa * 16 * 4 + rotacijaElementa * 16; // biramo element iz stringa
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            bool moze = Elementi[izaberi] == '0' ? false : true;
            if(moze){
                if(i+trenutniElement.elementY > 19) return false; // ako bi novi element presao duzinu table, ne moze da se rotira
                if(j+trenutniElement.elementX > 9) return false; // ako bi novi element presao sirinu table, ne moze da se  rotira
                // ako se na tom mestu vec nesto nalazi ne moze da se rotira
                if(tabla[trenutniElement.elementY + i][trenutniElement.elementX + j] !=0) return false;
            }
            // prelazimo na sledecu celiju
            izaberi++;
        }
    return true; // inace vracamo true i moze da se rotira
}


// fja proverava da li je kraj igre
void Logika::krajIgre(){
    if(rezultat<=0){ // ako je rezultat manji ili jednak nuli onda je to kraj
        kraj=true; // flag kraj se postavlja na true
        mainApp->krajIgre(); // poziva se fja i mainwindow.cpp
    }
    else{ // inace...
        for(int i=0; i<10; i++) // proveravamo prvi red
            if(tabla[0][i]!=0){ // ako se u njemu nalazi deo nekog elementa
                kraj=true; // oznacavamo da je kraj igre
                mainApp->krajIgre(); // pozivamo fju iz mainwindow.cpp
                break;
            }
        }
}


// fja koja cisti red
void Logika::ciscenjeReda(){
    for(int k=19; k>=1; k--){ // krecemo od kraja
        int celija = 0; // neka je celija jednaka 0
        for(int i=0; i<10; i++) // krecemo se kroz red
            if(tabla[k][i]!=0)celija++; // ako se u odredjenom redu nalazi deo elementa, povecavamo broj celija
        if(celija==10){ // ako je broj celija jednak 10, onda je to pun red, i treba da se cisti
            for(int i=k; i>=1; i--) // ide se od pronadjenog reda
                for(int j=0; j<10; j++) //  za svaku od celija
                    tabla[i][j] = tabla[i-1][j]; // kopiramo red iznad u red ispod(tako funkcionise "padanje")
            rezultat-=100;// rezultat se umanjuje za 100
            cisti=true;
            k++;
        }
    }
}
