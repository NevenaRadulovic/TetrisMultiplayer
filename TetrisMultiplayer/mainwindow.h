#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resetujBrzinu();
    void krajIgre();
    QTcpSocket *socket;
public slots:
    void tajmer_igre();

private slots:
    void readyRead();
    void on_buttonStart_clicked();

    void on_buttonStop_clicked();
    void connected();
    void on_connect_clicked();

  //  void on_saljiButton_clicked();
private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    int brzina;

    QString clientId;
    bool firstConnection;
protected:
    void keyPressEvent(QKeyEvent *e);

};

#endif // MAINWINDOW_H
