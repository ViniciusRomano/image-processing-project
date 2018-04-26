// Vinicius Drago Romano - 1581767
#include "pdi.h"
#include <list>

PDI::PDI()
{

}

PDI::~PDI()
{

}

//Arithmetics functions

int median (int n[], int size) {

    for (int i = size - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (n[j] > n[j+1]) {
                int dTemp = n[j];
                n[j] = n[j+1];
                n[j+1] = dTemp;
            };
        };
    };

    double result = 0;

    if ((size % 2) == 0) {
        result = (n[size/2] + n[(size/2) - 1])/2.0;
    } else {
        result = n[size/2];
    };

    return result;
}

int max (int n[], int size) {
    int max = 0;
    for(int x=0; x<size;x++){
        if(max<n[x]){
            max = n[x];
        }
    }
    //printf(" - %d\n",max);
    return max;
}
int min (int n[], int size) {
    double min = 255;
    for(int x=0; x<size;x++){
        if(min>n[x]){
            min = n[x];
        }
    }
    return min;
}

void PDI::carregarImagem(String filename)
{
    //le a imagem
    imagemRGB = imread(filename,1);
    //caso a imagem tenha sido lida com sucesso
    if(imagemRGB.data)
    {
    //exibe a imagem na janela
    //imshow("Imagem Original", imagemRGB);
    }
    //CONVERSAO PARA ESCALA DE CINZA
    cvtColor(imagemRGB,imagemCINZA,COLOR_BGR2GRAY);
    //exibe a imagem cinza na janela
    imshow("Imagem Cinza", imagemCINZA);

}

void PDI::negativo()
{
     Mat negativo = imagemCINZA.clone();
     for(int x=0;x<negativo.rows;x++)
     {
         for(int y=0;y<negativo.cols;y++)
         {
             uchar pixel = negativo.at<uchar>(x,y);
             negativo.at<uchar>(x,y) = 255-pixel;
         }
     }
     //exibe a imagem na janela
     imshow("Negativo de uma Imagem", negativo);
}

void PDI::ajusteContraste()
{
     Mat contraste = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<contraste.rows;x++)
     {
         for(int y=0;y<contraste.cols;y++)
         {
             uchar pixel = contraste.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }
     //g=((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin
     uchar gmax=255; uchar gmin=0;
     for(int x=0;x<contraste.rows;x++)
     {
         for(int y=0;y<contraste.cols;y++)
         {
             uchar f = contraste.at<uchar>(x,y);
             contraste.at<uchar>(x,y) = ((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin;
         }
     }
     //exibe a imagem na janela
     imshow("Imagem Contraste", contraste);
}

void PDI::histograma()
{
    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    int h[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int nivel = 0;
    for(int x=0;x<imagemCINZA.rows;x++)
    {
        for(int y=0;y<imagemCINZA.cols;y++)
        {
            nivel = (int)imagemCINZA.at<uchar>(x,y);
            h[nivel] += 1;
        }
    }

    //normalizar vetor para plotar o grafico
    int maior=0;
    for(int i=0;i<256;i++)
    {
        if(h[i]>maior) maior=h[i];
    }
    int hn[256];
    for(int i=0;i<256;i++)
    {
        hn[i]=round(h[i]*255/maior);
    }
    //criar imagem em branco para o histrograma
    int altura=256; int largura=512;
    Mat imagemH(altura,largura,CV_8UC3,Scalar(255,255,255));
    Point pt1 = Point(0,0); Point pt2 = Point(0,0);
    for(int i=0; i<altura; i++)
    {
        pt1.x=i*2;pt1.y=altura-1;
        pt2.x=i*2;pt2.y=pt1.y-hn[i];
        line(imagemH,pt1,pt2,Scalar(255,100,50+hn[i]),1,8);
    }

    //exibe a imagem na janela
    imshow("Histograma", imagemH);
}

void PDI::logaritmica()
{
     Mat logImage = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<logImage.rows;x++)
     {
         for(int y=0;y<logImage.cols;y++)
         {
             uchar pixel = logImage.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }
     for(int x=0;x<logImage.rows;x++)
     {
         for(int y=0;y<logImage.cols;y++)
         {
             uchar f = logImage.at<uchar>(x,y);
             double a = 255/log(1+fmax);
             double result = a*log(f+1);
             logImage.at<uchar>(x,y) = (uchar)result;
         }
     }
     //exibe a imagem na janela
     imshow("Imagem Log", logImage);
}

void PDI::potencia()
{
     Mat p = imagemCINZA.clone();
     for(int x=0;x<p.rows;x++)
     {
         for(int y=0;y<p.cols;y++)
         {
             uchar f = p.at<uchar>(x,y);
             double a = 2; double c=1;
             double result = a*pow(f,c);
             p.at<uchar>(x,y) = (uchar)result;
         }
     }
     //exibe a imagem na janela
     imshow("Transf de Potencia ", p);
}

void PDI::equalizacaoHistograma()
{
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();
    
    float hist[256];
    int linhas = f.rows;
    int cols = f.cols;
    int size = linhas*cols;

    for(int x=0;x<256;x++){
        hist[x]=0;
    }
    //conta os valores de cada pixel
    for(int x=0;x<linhas;x++){
        for(int y=0;y<cols;y++){
            hist[(int)f.at<uchar>(x,y)] += 1;
        }
    }

    //normalizando
    for(int x=0;x<256;x++){
        hist[x]=hist[x]/size;
    }
    //acumulado
    for(int x=1; x<256; x++) {
        hist[x] = hist[x]+hist[x-1];
    }
    //temquedar1
    printf("%f",hist[255]);

    for(int x=0; x<256; x++) {
        hist[x] = int((255)*hist[x]);
    }

    // altera img de saida
    for(int x=0;x<linhas;x++){
        for(int y=0;y<cols;y++){
            g.at<uchar>(x,y) = (int)hist[(int)f.at<uchar>(x,y)];
        }
    }

    imshow("Imagem Equalizada", g);
}

void PDI::filtroMediana()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int aux = 0;
             int arr[9];
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) arr[aux] = f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) arr[aux] = f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) arr[aux] = f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else arr[aux] = f.at<uchar>(x+i,y+j);
                     aux++;
                 }
             }
             g.at<uchar>(x,y)=median(arr,9);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro da Mediana", g);
}

void PDI::filtroMedia()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int soma = 0;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) soma += f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) soma += f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) soma += f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else soma += f.at<uchar>(x+i,y+j);
                 }
             }
             g.at<uchar>(x,y)=soma/9;
         }
     }
     //exibe a imagem na janela

     imshow("Filtro da Media", g);
}

void PDI::filtroGauss(){

     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     int mat_gauss[5][5] ={{1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1}};
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int soma = 0;
             int soma2 = 0;
             //percorre uma mascara 3x3
             for(int i=-2; i<3; i++)
             {
                 for(int j=-2; j<3; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)){
                         soma += ((int)f.at<uchar>(x-i,y-j) * mat_gauss[i+2][j+2]);
                        // cout << mat_gauss[i+2][j+2];
                         soma2 += mat_gauss[i+2][j+2];
                        // printf("\n");
                     }
                     else if(((x+i)<0)||((x+i)>=f.rows)) {
                         soma += ((int)f.at<uchar>(x-i,y+j) * mat_gauss[i+2][j+2]);
                         //cout << mat_gauss[i+2][j+2];
                        // printf("\n");
                         soma2 += mat_gauss[i+2][j+2];
                     }
                     else if(((y+i)<0)||((y+i)>=f.cols)) {
                         soma += ((int)f.at<uchar>(x+i,y-j) * mat_gauss[i+2][j+2]);
                         //cout << mat_gauss[i+2][j+2];
                         //("\n");
                         soma2 += mat_gauss[i+2][j+2];
                     }
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else {
                         soma += ((int)f.at<uchar>(x+i,y+j) * mat_gauss[i+2][j+2]);
                         //cout << mat_gauss[i+2][j+2];
                         soma2 += mat_gauss[i+2][j+2];
                         //printf("\n");
                     }
                 }
             }
             //printf("---------------\n");
             //cout << soma2;
             g.at<uchar>(x,y)= int(soma/273);
             //cout << g.at<uchar>(x,y);
         }
     }
     //exibe a imagem na janela

     imshow("Filtro Gauss", g);
}

void PDI::filtroMin()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int aux = 0;
             int arr[9];
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) arr[aux] = f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) arr[aux] = f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) arr[aux] = f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else arr[aux] = f.at<uchar>(x+i,y+j);
                     aux++;
                 }
             }
             g.at<uchar>(x,y)= min(arr,9);
             //printf("%d\n",g.at<uchar>(x,y));
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Minimo", g);
}

void PDI::filtroMax()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++)
     {
         for(int y=0;y<f.cols;y++)
         {
             int aux = 0;
             int arr[9];
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++)
             {
                 for(int j=-1; j<2; j++)
                 {
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) arr[aux] = f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) arr[aux] = f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) arr[aux] = f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else arr[aux] = f.at<uchar>(x+i,y+j);
                     aux++;
                 }
             }
             g.at<uchar>(x,y)=max(arr,9);
         }
     }
     //exibe a imagem na janela
     imshow("Filtro do Maximo", g);
}

// Opencv functions (Test only)

void PDI::filtroMinOpencv()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();

     cv::Mat kernel;   // Use the default structuring element (kernel) for erode and dilate

     // Perform min filtering on image using erode
     cv::erode(f, g, kernel);

     //exibe a imagem na janela
     imshow("Filtro do Minimo OpenCv", g);
}

void PDI::filtroMaxOpencv()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();

     cv::Mat kernel;   // Use the default structuring element (kernel) for erode and dilate

     // Perform min filtering on image using erode
     cv::dilate(f, g, kernel);

     //exibe a imagem na janela
     imshow("Filtro do Maximo Opencv", g);
}

void PDI::filtroGaussOpencv()
{
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();

     cv::Mat kernel;   // Use the default structuring element (kernel) for erode and dilate

     // Perform min filtering on image using erode
     cv::GaussianBlur(f, g, Size( 5, 5 ),0,0);

     //exibe a imagem na janela
     imshow("Filtro Gauss Opencv", g);
}

void PDI::equalizacaoHistogramaOpencv()
{
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();


    // Perform min filtering on image using erode
    cv::equalizeHist(f, g);

    //exibe a imagem na janela
    imshow("Imagem Equalizada Opencv", g);
}


void PDI::brilhoHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];
    //percorre apenas o plano V
    uchar brilho=0;
    int k=50; //constante para aumentar ou diminuir o brilho
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            brilho = V.at<uchar>(x, y);
            if ((brilho+k) < 0) brilho = 0;
            else if ((brilho+k) > 255) brilho = 255;
            else brilho+=k;
            V.at<uchar>(x, y) = brilho;
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Ajuste de Brilho", imagemSaida);
}

void PDI::filtroMedianaHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    vector<Mat> planosHSV2;

    for(int x=0; x<3; x++) {
        planosHSV2.push_back(planosHSV[x]);
    }

    for(int z=0; z<3; z++){
         //percorre a imagem inteira
         for(int x=0;x<planosHSV[z].rows;x++)
         {
             for(int y=0;y<planosHSV[z].cols;y++)
             {
                 int aux = 0;
                 int arr[9];
                 //percorre uma mascara 3x3
                 for(int i=-1; i<2; i++)
                 {
                     for(int j=-1; j<2; j++)
                     {
                         //caso pixels da mascara estejam fora dos limites da imagem
                         //usa-se espelhamento de pixels
                         if(((x+i)<0 && (y+j)<0)||((x+i)>=planosHSV[z].rows && (y+j)>=planosHSV[z].cols)) arr[aux] = (int)planosHSV[z].at<uchar>(x-i,y-j);
                         else if(((x+i)<0)||((x+i)>=planosHSV[z].rows)) arr[aux] = (int)planosHSV[z].at<uchar>(x-i,y+j);
                         else if(((y+i)<0)||((y+i)>=planosHSV[z].cols))  arr[aux] = (int)planosHSV[z].at<uchar>(x+i,y-j);
                         //caso todos os pixels da mascara estejam dentro dos limites da imagem
                         else  arr[aux] = (int)planosHSV[z].at<uchar>(x+i,y+j);
                         aux++;
                     }
                 }
                 planosHSV2[z].at<uchar>(x,y)=median(arr,9);
             }
         }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV2,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro da MedianaHSV", imagemSaida);
}

void PDI::filtroGaussHSV(){
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);
    vector<Mat> planosHSV2;

    for(int x=0; x<3; x++) {
        planosHSV2.push_back(planosHSV[x]);
    }
    int mat_gauss[5][5] ={{1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1}};

         for(int x=0;x<planosHSV[2].rows;x++)
         {
             for(int y=0;y<planosHSV[2].cols;y++)
             {
                 int soma = 0;
                 //percorre uma mascara 5x5
                 for(int i=-2; i<3; i++)
                 {
                     for(int j=-2; j<3; j++)
                     {
                         //caso pixels da mascara estejam fora dos limites da imagem
                         //usa-se espelhamento de pixels
                         if(((x+i)<0 && (y+j)<0)||((x+i)>=planosHSV[2].rows && (y+j)>=planosHSV[2].cols)) soma+= (int)planosHSV[2].at<uchar>(x-i,y-j)  * mat_gauss[i+2][j+2];
                         else if(((x+i)<0)||((x+i)>=planosHSV[2].rows)) soma+= (int)planosHSV[2].at<uchar>(x-i,y+j) * mat_gauss[i+2][j+2];
                         else if(((y+i)<0)||((y+i)>=planosHSV[2].cols))  soma+= (int)planosHSV[2].at<uchar>(x+i,y-j) * mat_gauss[i+2][j+2];
                         //caso todos os pixels da mascara estejam dentro dos limites da imagem
                         else  soma+= (int)planosHSV[2].at<uchar>(x+i,y+j) * mat_gauss[i+2][j+2];
                     }
                 }
                    planosHSV2[2].at<uchar>(x,y) = int(soma/273);
             }
         }

    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV2,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro da GaussHSV", imagemSaida);
}

void PDI::equalizacaoHistogramaHSV()
{
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    // Matriz // Vetor de matriz
    split(imagemHSV, planosHSV);

    float hist[256];

    int linhas = imagemHSV.rows;
    int cols = imagemHSV.cols;
    int size = linhas*cols;

    for(int x=0;x<256;x++){
        hist[x]=0;
    }
    //conta os valores de cada pixel

        for(int x=0;x<linhas;x++){
            for(int y=0;y<cols;y++){
                hist[(int)planosHSV[2].at<uchar>(x,y)] += 1;
            }
        }


    //normalizando

        for(int x=0;x<256;x++){
            hist[x]=hist[x]/size;
        }

    //acumulado

        for(int x=1; x<256; x++) {
            hist[x] = hist[x]+hist[x-1];
        }

        for(int x=0; x<256; x++) {
            hist[x] = int((255)*hist[x]);
        }

    // altera img de saida

        for(int x=0;x<linhas;x++){
            for(int y=0;y<cols;y++){
                planosHSV[2].at<uchar>(x,y) = (int)hist[(int)planosHSV[2].at<uchar>(x,y)];
                //cout << planosHSV.at<uchar>(x,y) << "\n";
            }
        }


    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro da GaussHSV", imagemSaida);
}
