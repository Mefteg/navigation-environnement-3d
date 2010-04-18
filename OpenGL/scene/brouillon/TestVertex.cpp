#include "vertex.h"
//#include "vertex.cpp"

float droiteGauche(float x, float y, float x0,float y0, float x1, float y1){
	return (y -y0)*(x1 - x0) - (x -x0)*(y1 - y0);
}

// retourne vrai si il est dedans ou sur la ligne
// on suppose que l'on travaille avec des carres (le fond de la bounding box)
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
int main(){
	Vertex p1(0, 3, 0, 0 );
	Vertex p2(0, 5, 2, 0);
	Vertex p3(0, 3, 4, 0);
	Vertex p4(0, 1, 2, 0);
	Vertex p(0, 0, 0, 0);

	if(vertexInsidePolyg( p1,  p2,  p3,  p4,  p))
		cout << "a l interieur";
	else
		cout << "a l exterieur";
}

