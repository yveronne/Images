#include <iostream>
#include "Image.h"


using namespace std;

int main()
{
    string folderPath = "/home/veronne/CLionProjects/Images/Photos/";
    string filename = "balloons.ascii"; //filename without extension
    Image myImage(folderPath+filename+".pgm");
//    Image myImage2(path);
//myImage.egalisationHistogramme("/home/veronne/CLionProjects/Images/Photos/ballonsEgalises.pgm");
//myImage.filtreLaplace("/home/veronne/CLionProjects/Images/Photos/ballonsLaplace.pgm");
//    myImage.filtreMoyenneur(folderPath+filename+"_filtreMoyenneur_7.pgm", 7);
    myImage.interpolationPlusProcheVoisin(folderPath+filename+"_interpolationPPV.pgm");
//    myImage.filtreGauss("/home/veronne/CLionProjects/Images/Photos/brainGauss.pgm");
    //myImage.soustraireImage(myImage2, "/home/veronne/CLionProjects/Images/balloonsSoustrait.pgm");
    myImage.desallocateMatrix();
    return 0;
}
