# RS019-tetris-multiplayer

Tetris Multiplayer

Program treba da simulira igru Tetris, ali tako da igra jedan igrač protiv drugog.
Svaki od igrača bi igrao klasičan tetris, ali sa malom izmenom, a to je da se poeni ne povećavaju počevši od 0.
Npr. na početku igre svaki od igrača bi imao 1000 poena.  
Svako "čišćenje" reda koje ostvari prvi igrač, drugom bi dodavalo 100 bodova na zbir, dok bi prvom igraču oduzimalo 100 bodova od zbira.
Ista pravila bi važila i za drugog igrača.
Pobednik je onaj ko prvi dođe do 0.

Program smo podelili tako da prvo pravimo klasican tetris, a nakon toga i deo koji će ovu igru da pretvori u multiplayer.

Grafički deo je ono od čega ćemo da krenemo.
Dakle, pravljenje polja za igru, tekst polja za ispis rezultata,labele koja ispisuje redosled konektovanja igraca. 
Tri button-a, prvi za startovanje igre, drugi za prekid igre, i treci za konektovanje na server.

Server je poseban deo, koji vodi racuna o raspodeli bodova,nakon sto izracuna trenutni rezultat, salje ga igracima.
Rezultat koji igrac dobije od servera se postavlja kao trenutni rezultat.


