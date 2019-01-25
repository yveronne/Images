//
// Created by veronne on 24/10/18.
//

#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

Image::Image()
{
    m_comment = "Image créée avec le constructeur par défaut";
    m_width = 90;
    m_height = 50;
}

Image::~Image()
{
    desallocateMatrix();
}

Image::Image(string path){
    m_path = path;
    ifstream flux(path);
    string ligne;
    int nbre;

    if(flux){
        getline(flux,ligne);
        getline(flux,ligne);
        m_comment = ligne;
        flux >> nbre;
        m_width = nbre;
        flux >> nbre;
        m_height = nbre;
        getline(flux, ligne);
        getline(flux, ligne);

        matrix = new int* [m_height];
        for(int j=0; j<m_height; j++){
            matrix[j] = new int [m_width];
        }

        for(int j=0; j<m_height; j++){
            for(int i=0; i<m_width; i++){
                flux >> nbre;
                matrix[j][i] = nbre;
            }
        }

    }
    else cout << "Une erreur est survenue lors de la lecture du fichier" << endl;
}

void Image::desallocateMatrix() {

    for(int i=0; i<m_height; i++){
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = 0;
}


vector<int> Image::getExtremePixels() {

    int** matrice = Image::Getmatrix();
    int height = Image::Getheight();
    int width = Image::Getwidth();
    int pixelMax = 0;
    int pixelMin = 255;
    int pixel;
    vector<int> extremePixels(2);


    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            pixel = matrice[i][j];
            if(pixel < pixelMin)    pixelMin = pixel;
            if(pixel > pixelMax)    pixelMax = pixel;
        }
    }
    extremePixels[0] = pixelMin;
    extremePixels[1] = pixelMax;

    return extremePixels;
}

void Image::transformationLineaire(string path) {

    int width = m_width;
    int height = m_height;
    int pixelMin = Image::getExtremePixels()[0];
    int pixelMax = Image::getExtremePixels()[1];

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            flux << 255*(matrix[i][j]- pixelMin)/(pixelMax - pixelMin) << endl;
        }
    }

}

void Image::transformationLineaireSaturation(string path, int sMin, int sMax) {

    int width = m_width;
    int height = m_height;
    int pixel;
    int pixelMin = Image::getExtremePixels()[0];
    int pixelMax = Image::getExtremePixels()[1];

    if (pixelMin < sMin) {
        cout << "La valeur de saturation minimale doit être supérieure ou égale à la valeur minimale"
                " des pixels de l'image. La valeur minimale des pixels est " << pixelMin << endl;
    } else if (pixelMax > sMax) {
        cout << "La valeur de saturation maximale doit être inférieure ou égale à la valeur maximale"
                " des pixels de l'image. La valeur maximale des pixels est " << pixelMax << endl;
    } else {
        ofstream flux(path);
        flux << "P2" << endl;
        flux << "# Creator : Véronne" << endl;
        flux << width << " " << height << endl;
        flux << 255 << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pixel = 255 * (matrix[i][j] - sMin) / (sMax - sMin);
                if (pixel <= 0) flux << 0 << endl;
                else if (pixel >= 255) flux << 255 << endl;
                else flux << pixel << endl;
            }
        }
    }
}

vector<int> Image::getHistogramme() {

    vector<int> histogramme(256, 0);
    int height = m_height;
    int width = m_width;
    int k=0;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            k = matrix[i][j];
            histogramme[k]++;
        }
    }
    return histogramme;

}

void Image::egalisationHistogramme(std::string path) {
    int width = m_width;
    int height = m_height;
    int nbrePixels = height*width;
    vector<int> histogramme = getHistogramme();
    vector<float> histogrammeNormalise(255);
    vector<float> densiteNormalisee(255);

    for(int i=0; i<256; i++){
        histogrammeNormalise[i] = (float)histogramme[i]/nbrePixels;
    }
    for(int i=0; i<256; i++){
        for(int j=0; j<=i; j++){
            densiteNormalisee[i] = densiteNormalisee[i] + histogrammeNormalise[j];
        }
    }

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            flux << (int)(densiteNormalisee[matrix[i][j]]*255) << endl;
        }
    }

}

void Image::ajouterImage(Image image, string path) {

    if(Getheight()!= image.Getheight() || Getwidth()!= image.Getwidth()){
        cout << "Les deux images n'ont pas la même taille!" << endl;
    }
    else{
        int** matrice = matrix;
        int** matrice2 = image.Getmatrix();
        int height = m_height;
        int width = m_width;
        int pixel;

        ofstream flux(path);
        flux << "P2" << endl;
        flux << "# Creator : Véronne" << endl;
        flux << width << " " << height << endl;
        flux << 255 << endl;

        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                pixel = matrice[i][j] + matrice2[i][j];
                if(pixel > 255)     pixel = 255;
                flux << pixel << endl;
            }
        }
    }

}

void Image::soustraireImage(Image image, string path) {

    if(Getheight()!= image.Getheight() || Getwidth()!= image.Getwidth()){
        cout << "Les deux images n'ont pas la même taille!" << endl;
    }
    else{
        int** matrice = matrix;
        int** matrice2 = image.Getmatrix();
        int height = m_height;
        int width = m_width;
        int pixel;

        ofstream flux(path);
        flux << "P2" << endl;
        flux << "# Creator : Véronne" << endl;
        flux << width << " " << height << endl;
        flux << 255 << endl;

        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                pixel = matrice[i][j] - matrice2[i][j];
                if(pixel < 0)     pixel = 0;
                flux << pixel << endl;
            }
        }
    }

}

void Image::multiplierImage(float ratio, std::string path) {

    int height = m_height;
    int width = m_width;
    int pixel;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            pixel = (int)(Getmatrix()[i][j]*ratio);
            if(pixel > 255)     pixel = 255;
            flux << pixel << endl;
        }
    }

}


void Image::interpolationPlusProcheVoisin(std::string path) {
    int width = m_width;
    int height = m_height;
    int newWidth = 2*width;
    int newHeight = 2*height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << newWidth << " " << newHeight << endl;
    flux << 255 << endl;

    for(int j=0; j<width; j++){
        flux << matrix[0][j] << endl;
        flux << matrix[0][j] << endl;
    }

    for(int i=1; i<newHeight; i++){
        if(i%2 != 0){
            for(int j=0; j<newWidth; j++){
                flux << matrix[i/2][j/2] << endl;
            }
        }
        else{
            for(int j=0; j<width; j++){
                flux << matrix[i/2][j] << endl;
                flux << matrix[i/2][j] << endl;
            }
        }
    }

}



void Image::filtreGauss(std::string path){

    int width = m_width;
    int height = m_height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for (int i=0; i<2*width; i++){
        flux << 0 << endl;
    }
    for (int i=2; i<height-2; i++){
        flux << 0 << endl;
        flux << 0 << endl;
        for(int j=2; j<width-2; j++){
            flux << (10*matrix[i][j] + matrix[i-2][j-2] + matrix[i+2][j-2] + matrix[i-2][j+2]
                        + matrix[i+2][j+2] + 2*(matrix[i-1][j-2] + matrix[i+1][j-2] + matrix[i-2][j-1]
                        + matrix[i+2][j-1] + matrix[i-2][j+1] + matrix[i+2][j+1] + matrix[i-1][j+2] + matrix[i+1][j+2])
                        + 3*(matrix[i][j-2] + matrix[i-2][j] + matrix[i+2][j] + matrix[i][j+2])
                        + 6*(matrix[i-1][j-1] + matrix[i+1][j-1] + matrix[i-1][j+1] + matrix[i+1][j+1])
                        + 8*(matrix[i][j-1] + matrix[i-1][j] + matrix[i+1][j] + matrix[i][j+1]))/98  << endl;
        }
        flux << 0 << endl;
        flux << 0 << endl;
    }

    for (int i=0; i<2*width; i++){
        flux << 0 << endl;
    }
}

void Image::filtreMoyenneur(std::string path, int ordre){

    int width = m_width;
    int height = m_height;
    int decalageMax = ordre/2;
    int pixel=0;
    int carre = ordre*ordre;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for(int k=0; k<decalageMax; k++){
        for (int i=0; i<width; i++){
            flux << 0 << endl;
        }
    }

    for (int i=decalageMax; i<height-decalageMax; i++){
        for(int k=0; k<decalageMax; k++){
            flux << 0 << endl;
        }
        for(int j=decalageMax; j<width-decalageMax; j++){
            for(int k=0; k<ordre; k++){
                for(int l=0; l<ordre; l++){
                    pixel = pixel+ matrix[i+k-decalageMax][j+l-decalageMax];
                }
            }
            flux << pixel/carre << endl;
            pixel=0;
        }
        /*for(int j=decalageMax; j<width-1; j++){
            for(int k=i-decalageMax; k<i+decalageMax; k++){
                for (int l=j-decalageMax; l<j+decalageMax; l++){
                    pixel += matrix[k][l];
                }
            }
            flux << pixel/carre << endl;
        }*/
        for(int k=0; k<decalageMax; k++){
            flux << 0 << endl;
        }
    }

    for(int k=0; k<decalageMax; k++){
        for (int i=0; i<width; i++){
            flux << 0 << endl;
        }
    }
}

void Image::filtreMedian(std::string path) {

    int width = m_width;
    int height = m_height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
    for (int i=1; i<height-1; i++){
        flux << 0 << endl;
        for(int j=1; j<width-1; j++){
            int pixel = matrix[i][j];
            vector<int> voisins(9,pixel);
            voisins[0] = matrix[i-1][j-1];
            voisins[1] = matrix[i-1][j];
            voisins[2] = matrix[i-1][j+1];
            voisins[3] = matrix[i][j-1];
            voisins[5] = matrix[i][j+1];
            voisins[6] = matrix[i+1][j-1];
            voisins[7] = matrix[i+1][j];
            voisins[8] = matrix[i+1][j+1];
            sort (voisins.begin(), voisins.end());
            flux << voisins[4] << endl;
        }
        flux << 0 << endl;
    }

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
}

void Image::filtrePrewitt(std::string path){

    int width = m_width;
    int height = m_height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
    for (int i=1; i<height-1; i++){
        flux << 0 << endl;
        for(int j=1; j<width-1; j++){
            flux << abs(- matrix[i-1][j-1] + matrix[i+1][j+1] - matrix[i-1][j]
                        + matrix[i+1][j] - matrix[i-1][j+1] + matrix[i+1][j-1])
                        + abs(- matrix[i-1][j-1] - matrix[i][j-1] + matrix[i][j+1] + matrix[i+1][j+1]
                               + matrix[i-1][j+1] + matrix[i+1][j-1])  << endl ;
        }
        flux << 0 << endl;
    }

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
}

void Image::filtreSobel(std::string path){

    int width = m_width;
    int height = m_height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
    for (int i=1; i<height-1; i++){
        flux << 0 << endl;
        for(int j=1; j<width-1; j++){
            flux << abs(- matrix[i-1][j-1] + matrix[i+1][j+1] - 2*matrix[i-1][j]
                        + 2*matrix[i+1][j] - matrix[i-1][j+1] + matrix[i+1][j-1])
                    + abs(- matrix[i-1][j-1] - 2*matrix[i][j-1] + 2*matrix[i][j+1] + matrix[i+1][j+1]
                          + matrix[i-1][j+1] + matrix[i+1][j-1])  << endl ;
        }
        flux << 0 << endl;
    }

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
}

void Image::filtreLaplace(std::string path){

    int width = m_width;
    int height = m_height;

    ofstream flux(path);
    flux << "P2" << endl;
    flux << "# Creator : Véronne" << endl;
    flux << width << " " << height << endl;
    flux << 255 << endl;

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
    for (int i=1; i<height-1; i++){
        flux << 0 << endl;
        for(int j=1; j<width-1; j++){
            flux << abs(-8*matrix[i][j] + matrix[i-1][j-1] + matrix[i+1][j+1] + matrix[i-1][j]
                        + matrix[i][j-1] + matrix[i][j+1] + matrix[i+1][j] + matrix[i-1][j+1] + matrix[i+1][j-1]) << endl ;
        }
        flux << 0 << endl;
    }

    for (int i=0; i<width; i++){
        flux << 0 << endl;
    }
}

