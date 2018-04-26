// Vinicius Drago Romano - 1581767
#ifndef PDI_H
#define PDI_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


class PDI
{
    public:
        //metodos
        PDI();
        ~PDI();
        void carregarImagem(String filename);
        void negativo();
        void logaritmica();
        void potencia();
        void ajusteContraste();
        void histograma();
        void equalizacaoHistograma();
        void equalizacaoHistogramaHSV();
        void filtroMedia();
        void filtroMediana();
        void filtroMax();
        void filtroMin();
        void filtroGauss();
        void brilhoHSV();
        void filtroMedianaHSV();
        void filtroGaussHSV();
        //atributos
        Mat imagemRGB;
        Mat imagemCINZA;

        //functions OpenCv (only test)
        void filtroMaxOpencv();
        void filtroMinOpencv();
        void filtroGaussOpencv();
        void equalizacaoHistogramaOpencv();
};

#endif // PDI_H
