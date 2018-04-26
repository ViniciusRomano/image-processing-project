// Vinicius Drago Romano - 1581767

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pdi = new PDI();
}



MainWindow::~MainWindow()
{
    delete ui;
    delete pdi;
}

void MainWindow::on_actionAbrir_Imagem_triggered()
{
    //abre uma caixa de dialogo para selecao de uma imagem
    QString filename = QFileDialog::getOpenFileName(
    this, "Abrir", "/home/romano/Pictures",
    tr("Images(*.png *.jpg *.jpeg)")
    );
    pdi->carregarImagem(filename.toStdString());
}

void MainWindow::on_actionHistograma_triggered()
{
    pdi->histograma();
}

void MainWindow::on_actionNegativo_triggered()
{
    pdi->negativo();
}

void MainWindow::on_actionAjuste_de_Contraste_triggered()
{
    pdi->ajusteContraste();
}

void MainWindow::on_actionTransformacao_Logaritmica_triggered()
{
    pdi->logaritmica();
}

void MainWindow::on_actionTransformacao_de_Potencia_triggered()
{
    pdi->potencia();
}

void MainWindow::on_actionEqualizacaoHistograma_triggered()
{
    pdi->equalizacaoHistogramaOpencv();
    pdi->equalizacaoHistograma();
}

void MainWindow::on_actionMedia_triggered()
{
    pdi->filtroMedia();
}

void MainWindow::on_actionMediana_triggered()
{
    pdi->filtroMediana();
}

void MainWindow::on_actionMedianaCor_triggered()
{
    pdi->filtroMedianaHSV();
}


void MainWindow::on_actionMax_triggered()
{
    pdi->filtroMax();
    pdi->filtroMaxOpencv();
}

void MainWindow::on_actionMin_triggered()
{
    pdi->filtroMin();
    pdi->filtroMinOpencv();
}

void MainWindow::on_actionGauss_triggered()
{
    pdi->filtroGauss();
    pdi->filtroGaussOpencv();
}

void MainWindow::on_actionGaussCor_triggered()
{
    pdi->filtroGaussHSV();
}

void MainWindow::on_actionBrilho_triggered()
{
    pdi->brilhoHSV();
}

void MainWindow::on_actionHist_triggered()
{
    pdi->equalizacaoHistogramaHSV();
}
