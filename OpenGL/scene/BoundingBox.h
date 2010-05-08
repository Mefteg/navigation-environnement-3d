#ifndef  BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "forme.h"

using namespace std;

class BoundingBox {
	private:
		Vertex p1;
		Vertex p2;
		Vertex p3;
		Vertex p4;

	public:
		Vertex getP1(void);
		Vertex getP2(void);
		Vertex getP3(void);
		Vertex getP4(void);
		BoundingBox(Forme f);
};

// renvoie la liste des BoundingBox
vector <BoundingBox> formeToBoundingBox(vector<Forme> * vFormes);

// teste si le vertex est dans les BoundingBox
bool vertexInsideBoundingBox(vector <BoundingBox> bb, Vertex p);
// teste si l'arete est dans les BoundingBox
bool segmentIntersectBoundingBox(vector <BoundingBox> liBB, Vertex p1, Vertex p2);	// On peux l'appliquer si il manque un des voisins, c'est un peu tricher car c'est pas toujours vrai, mais ca peu faire gagner beaucoup de temps

#endif

