#include "BoundingBox.h"

// Accesseur
Vertex BoundingBox :: getP1(void){
	return p1;
}

Vertex BoundingBox :: getP2(void){
	return p2;
}

Vertex BoundingBox :: getP3(void){
	return p3;
}

Vertex BoundingBox :: getP4(void){
	return p4;
}


// Constructeurs :

/*
	On pourra ajouter facilement quatres points suppémentaires pour faire une vrai Bounding box
	et on fera les tests sur la hauteur
*/
BoundingBox :: BoundingBox(Forme f){
	vector<Vertex> *listePoints = f.getVertices();
	if(listePoints->size() < 3){
		cout << "La forme ne contient pas assez de points\n";
		return;
	}
	
	float xmin = listePoints->front().getX();
	float ymin = listePoints->front().getY();
	//float zmin = listePoints->front().getZ();

	float xmax = xmin;
	float ymax = ymin;
	//float zmax = zmin;
	
	//on chercher les mins et max
	for(int i = 1; i < listePoints->size() ; i++){
		if(listePoints->at(i).getX() < xmin) 
			xmin = listePoints->at(i).getX();

		if(listePoints->at(i).getX() > xmax)
			xmax = listePoints->at(i).getX();

		if(listePoints->at(i).getY() < ymin) 
			ymin = listePoints->at(i).getY();

		if(listePoints->at(i).getY() > xmax) 
			ymax = listePoints->at(i).getY();

/*		if(listePoints->at(i).getZ() < zmin)
			zmin = listePoints->at(i).getZ();

		if(listePoints->at(i).getZ() > zmax)
			zmax = listePoints->at(i).getZ();*/
	}
	
	p1.setX(xmin);
	p1.setY(ymin);
	//p1.setZ();
	
	p2.setX(xmin);
	p2.setY(ymax);
	//p2.setZ();
	
	p3.setX(xmax);
	p3.setY(ymax);
	//p3.setZ();
	
	p4.setX(xmax);
	p4.setY(ymin);
	//p4.setZ();
}

vector <BoundingBox> BoundingBox :: formeToBoundingBox(vector<Forme> * vFormes){
	vector <BoundingBox> listeBoundingBox;
	
	vector<Forme>::iterator it;

	for ( it = vFormes->begin() ; it < vFormes->end(); it++ )
		listeBoundingBox.push_back(BoundingBox(*it));

}

// Fonction qui teste de quel cote du segment se situe un point
// Si la fonction retourne 0 alors le point est sur le segment
float droiteGauche(float x, float y, float x0,float y0, float x1, float y1){
    return (y -y0)*(x1 - x0) - (x -x0)*(y1 - y0);
}

// Retourne vrai si le point teste est dans le polygone (ici on a simplifie on travaille avec des carres donc un polygone Ã  4 sommets)
// L'algorithme va regarder ou se situe le point par rapport aux 4 cotes du polygone si il est toujours du meme cote alors le point est dans le polygone
// Sinon le point est dehors.
// On suppose que l'on travaille avec des carres (le fond de la bounding box)
bool vertexInsidePolyg(Vertex p1, Vertex p2, Vertex p3, Vertex p4, Vertex p){	// on teste si "p" est dans le carre p1p2p3p4

    // PREMIER TEST
    float res = droiteGauche(p.getX(), p.getY(), p1.getX(), p1.getY(), p2.getX(), p2.getY());

    if(res == 0)
	return true;
    bool signe = res > 0;	// vrai si positif

    // DEUXIEME TEST
    res = droiteGauche(p.getX(), p.getY(), p2.getX(), p2.getY(), p3.getX(), p3.getY());

    if(res == 0)
	return true;

    bool signe2 = res > 0;	// vrai si positif
    if(signe != signe2)
	return false;	// a l'exterieur

    // TROISIEME TEST
    res = droiteGauche(p.getX(), p.getY(), p3.getX(), p3.getY(), p4.getX(), p4.getY());

    if(res == 0)
	return true;

    signe2 = res > 0;	// vrai si positif
    if(signe != signe2)
	return false;	// a l'exterieur

    // QUATRIEME TEST
    res = droiteGauche(p.getX(), p.getY(), p4.getX(), p4.getY(), p1.getX(), p1.getY());

    if(res == 0)
	return true;

    signe2 = res > 0;	// vrai si positif
    if(signe != signe2)
	return false;	// a l'exterieur

    // tous dans le meme sens => a l'interieur
    return true;
}

bool vertexInsideBoundingBox(BoundingBox bb, Vertex p){
	return vertexInsidePolyg(bb.getP1(), bb.getP2(), bb.getP3(), bb.getP4(), p);
}

bool vertexInsideBoundingBox(vector <BoundingBox> liBB, Vertex p){
	int i = liBB.size();
	while(i > 0){
		i--;
		if(vertexInsideBoundingBox(liBB.at(i), p))
			return true;
	}
	return false;
}

bool segmentsIntersection(Vertex a, Vertex b, Vertex c, Vertex d) {

	// Pour comprendre : http://www.faqs.org/faqs/graphics/algorithms-faq/

	    float Ax = a.getX();
	    float Ay = a.getY();
	    float Bx = b.getX();
	    float By = b.getY();
	    float Cx = c.getX();
	    float Cy = c.getY();
	    float Dx = d.getX();
	    float Dy = d.getY();

	    float r =((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) / ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));
	    float s = ((Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay)) / ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));

	return  (((0 <= r) and (r <= 1)) && ((0 <= s) and (s <= 1))); // vrai intersection faux pas d'intersection !
}

// Les points sont supposes etre ranges - Renvoie vrai si le segment intersecte la bounding box
bool segmentIntersectPolyg(Vertex a, Vertex b, Vertex c, Vertex d, Vertex p1, Vertex p2){
	return (segmentsIntersection(p1,p2,a,b) or segmentsIntersection(p1,p2,b,c) or segmentsIntersection(p1,p2,c,d) or segmentsIntersection(p1,p2,d,a));
}

bool segmentIntersectBoundingBox(BoundingBox bb, Vertex p1, Vertex p2){
	return segmentIntersectPolyg(bb.getP1(), bb.getP2(), bb.getP3(), bb.getP4(), p1, p2);
}

bool segmentIntersectBoundingBox(vector <BoundingBox> liBB, Vertex p1, Vertex p2){
	int i = liBB.size();
	while(i > 0){
		i--;
		if(segmentIntersectBoundingBox(liBB.at(i), p1, p2))
			return true;
	}
	return false;
}

int main(){}