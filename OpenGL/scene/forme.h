#ifndef FORME_H
#define FORME_H

#include <vector>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "vertex.h"
#include "face.h"

using namespace std;

class Forme {
	private:
		int num;
		vector<Vertex> vertices;
		vector<Face> faces;


	public:
		Forme();
		Forme( int num );

		int getNum();
		vector<Vertex> * getVertices();
		vector<Face> * getFaces();
		Vertex * getVertex( int numVertex );

		void setNum(int n);
		void setVertices( vector<Vertex> * vVertex );
		void setFaces( vector<Face> * vFaces );

		void addVertex( Vertex v );
		void addFace( Face f );

		//void removeVertex( int n );
		//void removeFace( int n );

        void findVoisinsVertex( Vertex * v, Face * f );
        void generateGraph();

		void draw();
		//void drawFace( int num_face );
		//string toString();
        string printGraph();
};

#endif

