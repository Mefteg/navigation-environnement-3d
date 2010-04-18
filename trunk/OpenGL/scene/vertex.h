#ifndef VERTEX_H
#define VERTEX_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>

//#include "face.h" sert à quelque chose ici ?

using namespace std;

class Vertex {
    private:
	int num;
	float x;
	float y;
	float z;
	vector<Vertex *> voisins;
	int visite;

    public:
	Vertex();
	Vertex( int num, float x, float y, float z );

	int getNum();
	float getX();
	float getY();
	float getZ();
	vector<Vertex *> * getVoisins();
	int getVisite();

	void setNum(int n);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setVisite( int i );

	void addVoisin( Vertex * v );
	void removeVoisin( int n );
	bool dejaVoisin( int p );
	void parcoursVoisins();
	int estIsole();	//retourne 1 si le sommet est isolé, 0 sinon
	string printVoisins();

	void draw( int r, int g, int b );
};

#endif
