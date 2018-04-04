#include "mainwindow.h"
#include "fileshandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    //QApplication::addLibraryPath("C:/Qt/5.9.1/mingw53_32/plugins/imageformats");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

//**************************************************************************************************//
// Authors: Tomasz Krzeszowski																		//
//																									//
// Version: 0.1.1 																					//
// Date: 27.07.2012																					//
//																									//												
//**************************************************************************************************//
/*#pragma comment(lib, "Model3D.lib")

#include <QApplication>
#include <iostream>

#include "Model3D.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	cout << endl
		<< "Program demonstruje dzialanie modelu 3D postaci ludzkiej. \n"
		<< "Przedstawiony model jest wykorzystywany podczas estymacji \naktualnej konfiguracji postaci ludzkiej." << endl;

	vector<vector<float> > modelState; //!< stany modelu
	vector<vector<pf::Vec3f> > vertices; //!< wierzcholki modelu
	vector<vector<float> >  radius; //!< promienie walcow modelu

									//utworzenie modelu zbudowanego z cylindrow, model znajduje sie w domyslnym ustawieniu (T-Pose)
	pf::Model3D model3D = pf::Model3D(pf::Model3D::Cylinder, "../ETC/model.asf", "../ETC/model.dat");

	//wczytanie wektorow stanu z pliku .amc do wektora ze stanami modelu
	pf::MotionData::loadStateVectorFromAMC("../ETC/model.amc", model3D.getNamesMovingBones(), modelState);
	//stan calego modelu mozemy ustawic przy pomocy wektora stanu
	model3D.setModelState(modelState[0]);
	//stan poszczegolnych kosci ustawiamy przy pomocy funkcji makeRotate
	model3D.makeRotate("LeftArm", DEG_TO_RAD*45.0f, pf::Model3D::axisX);

	//STICK model
	//w wektorze z wierzcholkami mamy wierzcholki oznaczajace poczatek oraz koniec dla kazdej z kosci, dane te moga zostac wykorzystane do 
	//zaprezentowania modelu w postaci stick model dane wierzcholkow sa zapisane w nastepujacej postaci vertices[indeksKosci][indeksWierzcholka - 0 lub 1]
	//postac ta tyczy sie jedynie modelu zbudowanego z cylindrow (w przypadku modelu zbudowanego z prostopadloscianow kazdy element ma 4 wierzcholki)
	vertices = model3D.getRotatedVertices();
	//w przypadku jesli chcemy wyrenderowac model zbudowany z walcow potrzebujemy jeszcze promieni dla poszczegolnych elementow modelu
	radius = model3D.getRadiusVec();

	//wypisanie wierzcholkow (lista zawiera tylko widoczne kosci)
	for (int i = 0; i<vertices.size(); ++i)
	{
		cout << "Kosc: " << i << endl;
		cout << "Pierwszy wierzcholek kosci (znajdujacy sie przy rodzicu)  x: " << vertices[i][0].x() << " y: " << vertices[i][0].y() << " z: " << vertices[i][0].z() << endl;
		cout << "Drugi wierzcholek kosci (znajdujacy sie przy potomku)  x: " << vertices[i][1].x() << " y: " << vertices[i][1].y() << " z: " << vertices[i][1].z() << endl;
	}

	return a.exec();
}
*/