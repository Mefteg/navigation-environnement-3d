#ifndef VERTEX_H
#define VERTEX_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "face.h"

using namespace std;

class Vertex {
	private:
		int num;
		float x;
		float y;
		float z;
        vector<int> voisins;

	public:
		Vertex();
		Vertex( int num, float x, float y, float z );

		int getNum();
		float getX();
		float getY();
		float getZ();
        vector<int> * getVoisins();

		void setNum(int n);
		void setX(float x);
		void setY(float y);
		void setZ(float z);

        void addVoisin( int n );
        void removeVoisin( int n );
        void findVoisins( Face * f );
        bool dejaVoisin( int p );
        string printVoisins();
};

#endif
