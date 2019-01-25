//
// Created by veronne on 24/10/18.
//

#ifndef IMAGES_IMAGE_H
#define IMAGES_IMAGE_H
#include <vector>
#include <string>


class Image {

public:

    Image();
    Image(std::string path);

    virtual ~Image();

    std::string Getpath() { return m_path; }
    void Setpath(std::string val) { m_path = val; }
    std::string Getcomment() { return m_comment; }
    void Setcomment(std::string val) { m_comment = val; }
    int Getwidth() { return m_width; }
    void Setwidth(int val) { m_width = val; }
    int Getheight() { return m_height; }
    void Setheight(int val) { m_height = val; }
    int** Getmatrix(){ return matrix;}
    void Setmatrix(int** val) { matrix = val; }

    void desallocateMatrix();
    void transformationLineaire(std::string path);
    void transformationLineaireSaturation(std::string path, int sMin, int sMax);
    std::vector<int> getHistogramme();
    void egalisationHistogramme(std::string path);

    void ajouterImage(Image image, std::string path);
    void soustraireImage(Image image, std::string path);
    void multiplierImage(float ratio, std::string path);

    void interpolationPlusProcheVoisin(std::string path);

    void filtreGauss(std::string path);
    void filtreMoyenneur(std::string path, int ordre);
    void filtreMedian(std::string path);

    void filtrePrewitt(std::string path);
    void filtreSobel(std::string path);
    void filtreLaplace(std::string path);

    std::vector<int> getExtremePixels();

protected:

private:
    std::string m_path;
    std::string m_comment;
    int m_width;
    int m_height;
    int **matrix;

};


#endif //IMAGES_IMAGE_H
