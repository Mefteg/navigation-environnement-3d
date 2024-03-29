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
	vector<Vertex> boundingBox;


    public:
	Forme();
	Forme( int num );

	int getNum();
	vector<Vertex> * getVertices();
	vector<Face> * getFaces();
	vector<Vertex> * getBoundingBox();
	Vertex * getVertex( int numVertex );

	void setNum(int n);
	void setVertices( vector<Vertex> * vVertex );
	void setFaces( vector<Face> * vFaces );
	void setBoundingBox( vector<Vertex> * vBoundingBox );

	void addVertex( Vertex v );
	void addFace( Face f );

	//void removeVertex( int n );
	//void removeFace( int n );

	void findVoisinsVertex( Vertex * v, Face * f );
	void generateGraph();
	void removeSommet( Vertex * v );
	void parcoursGraph();
	int generateBoundingBox();

	void draw();
	//void drawFace( int num_face );
	//string toString();
	string printGraph();
};

#endif

