#include "vertex.h"
//#include "vertex.cpp" // pour que marc compile :P 

// Fonction qui teste de quel côté du segment se situe un point
// Si la fonction retourne 0 alors le point est sur le segment
float droiteGauche(float x, float y, float x0,float y0, float x1, float y1){
	return (y -y0)*(x1 - x0) - (x -x0)*(y1 - y0);
}

// Retourne vrai si le point testé est dans le polygone (ici on a simplifié on travaille avec des carrés donc un polygone à 4 sommets)
// L'algorithme va regarder où se situe le point par rapport aux 4 cotés du polygone si il est toujours du même côté alors le point est dans le polygone
// Sinon le point est dehors.
// On suppose que l'on travaille avec des carrés (le fond de la bounding box)
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

