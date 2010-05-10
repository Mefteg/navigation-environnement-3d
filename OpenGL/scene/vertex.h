#ifndef VERTEX_H
#define VERTEX_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>

//#include "face.h"
//#include "BoundingBox.h"

using namespace std;

class Vertex {
    private:
	int num;
	float x;
	float y;
	float z;
	int r;
	int v;
	int b;
	vector<Vertex *> voisins;
	int visite;
	int poids;

    public:
	Vertex();
	Vertex( int num, float x, float y, float z );

	int getNum();
	float getX();
	float getY();
	float getZ();
	vector<Vertex *> * getVoisins();
	int getVisite();
	int getPoids();

	void setNum(int n);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setVisite( int i );
	void setRVB(int cr, int cv, int cb);
	void setPoids( int poids );

	int nbVoisins();
	void addVoisin( Vertex * v );
	void removeVoisin( int n );
	void removeDirectVoisin( int n ) ;
	bool dejaVoisin( int p );
	void isolation();
	void parcoursVoisins();
	void parcoursVoisinsDessiner();
	void parcoursVoisinsMerging( int profondeur );
	//void parcoursVoisinsSuppressionArete(vector <BoundingBox> liBB);
	//retourne 1 si le sommet est isolé, 0 sinon
	int estIsole();
	//retourne le poids du parcours
	int poidsParcours( int pas, int pere );
	int poidsParcours2( int pas );
	string printVoisins();

	void draw();

	string toString();
};
#endif
