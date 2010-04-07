#ifndef VERTEX_H
#define VERTEX_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "face.h"
#include "forme.h"

using namespace std;

class Vertex {
	private:
		int num;
		float x;
		float y;
		float z;
        vector<Vertex *> voisins;

	public:
		Vertex();
		Vertex( int num, float x, float y, float z );

		int getNum();
		float getX();
		float getY();
		float getZ();
        vector<Vertex *> * getVoisins();

		void setNum(int n);
		void setX(float x);
		void setY(float y);
		void setZ(float z);

        void addVoisin( Vertex * v );
        void removeVoisin( int n );
        void findVoisins( Face * f, Forme * maForme );
        bool dejaVoisin( int p );
        string printVoisins();
};

#endif
