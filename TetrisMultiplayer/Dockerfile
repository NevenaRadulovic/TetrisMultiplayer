# Specificna verzija Ubuntu-a za doslednost
FROM ubuntu:20.04

# Postavljanje okruzenja da bi se izbegli interaktivni promptrovi
ENV DEBIAN_FRONTEND=noninteractive

# Instalacija potrebnih paketa
RUN apt update -y 
RUN apt install -y git g++ make qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtmultimedia5-dev

# Kloniranje git repozitorijuma i priprema build repozitorijuma
RUN git clone https://github.com/NevenaRadulovic/TetrisMultiplayer.git /usr/src/tetris 
RUN mkdir /usr/src/tetris/build

# Definisanje radnog direktorijuma
WORKDIR /usr/src/tetris/build

# Kompiliranje projekta TetrisMultiplayer
RUN qmake ../TetrisMultiplayer/TetrisMultiplayer.pro && make

# Postavljanje komande koja ce se izvrsiti kada se kontejner pokrene
CMD ["./TetrisMultiplayer"]
