#include "../vertex.h"
#include "../vertex.cpp" // pour que marc compile :P 


#include "../face.h"
#include "../face.cpp"

#include "../forme.h"
#include "../forme.cpp"

#include <cmath>
#include <iostream>

using namespace std;

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

    cout << "r = " << ((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) << " / " << ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx)) << endl;
    cout << "s = " << ((Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay)) << " / " << ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx))<< endl;

    float r =((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) / ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));
    float s = ((Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay)) / ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));

    cout << "Après calcul : r = "<< r << " s = " << s << endl;

    return  (((0 <= r) and (r <= 1)) && ((0 <= s) and (s <= 1))); // vrai intersection faux pas d'intersection !

}

// Les points sont supposés être rangés - Renvoie vrai si le segment intersecte la bounding box
bool intersectionSegmentBoundingBox(Vertex p1, Vertex p2, Vertex a, Vertex b, Vertex c, Vertex d) {
    return (segmentsIntersection(p1,p2,a,b) or segmentsIntersection(p1,p2,b,c) or segmentsIntersection(p1,p2,c,d) or segmentsIntersection(p1,p2,d,a));
}

// Prend une face et la divise en 4 faces
void scission(Forme * f, int numero) {

    vector<Face> * pFaces; // la liste des faces de la forme f

    pFaces = f->getFaces();

    // Petit test pour vérifier si c'est pas vide
    if(pFaces->empty()) {
	cout << "Vecteur pFaces vide" << endl;
    }
    else {

	// On parcourt la liste des faces à la recherche de la face à scisser
	for(int i=0; i < pFaces->size() ; i++) {
	    if (pFaces->at(i).getNum() == numero) {

		// pFaces->at(i) = la face qu'on cherche à scisser

		vector<int> * sommets; //  contient les numéros des sommets de la face recherchée

		sommets = pFaces->at(i).getSommets();


		// On regarde si la face est un rectangle => on regarde si les 4 angles sont égaux à 90° (pas fait)
		if(sommets->size() == 4) {

		    // But = supprimer la face de la forme et on en créer 4

		    // On récupère les 4 points 
		    Vertex * p1 = f->getVertex(sommets->at(0));
		    Vertex * p2 = f->getVertex(sommets->at(1));
		    Vertex * p3 = f->getVertex(sommets->at(2));
		    Vertex * p4 = f->getVertex(sommets->at(3));

		    // p12 = p1 + p2 / 2 (Je ne suis pas sûr à 100% mais la formule me semble bonne !)
		    Vertex p12(0,0,0,0);
		    p12.setX((p1->getX() + p2->getX()) / 2); // x
		    p12.setY((p1->getY() + p2->getY()) / 2); // y 
		    p12.setZ((p1->getZ() + p2->getZ()) / 2); // z
		    p12.setNum(5); // CODER EN DUR ICI ! DOIT ETRE CHANGE ! J'avais pensé à récupérer le nombre total de vertex puis d'ajouter 1...

		    cout << "p12" << endl;
		    cout << p12.getX() << endl;
		    cout << p12.getY() << endl;
		    cout << p12.getZ() << endl<<endl;
		    
		    // p23 (idem p12)
		    Vertex p23(0,0,0,0);
		    p23.setX((p2->getX() + p3->getX()) / 2); 		    
		    p23.setY((p2->getY() + p3->getY()) / 2); 
		    p23.setZ((p2->getZ() + p3->getZ()) / 2);
		    p23.setNum(6);

		    cout << "p23" << endl;
		    cout << p23.getX() << endl;
		    cout << p23.getY() << endl;
		    cout << p23.getZ() << endl<<endl;
		    
		    // p34 
		    Vertex p34(0,0,0,0);
		    p34.setX((p3->getX() + p4->getX()) / 2); 
		    p34.setY((p3->getY() + p4->getY()) / 2); 
		    p34.setZ((p3->getZ() + p4->getZ()) / 2);
		    p34.setNum(7);

		    cout << "p34" << endl;
		    cout << p34.getX() << endl;
		    cout << p34.getY() << endl;
		    cout << p34.getZ() << endl<<endl;
		    
		    // p41 
		    Vertex p41(0,0,0,0);
		    p41.setX((p4->getX() + p1->getX()) / 2); 
		    p41.setY((p4->getY() + p1->getY()) / 2); 
		    p41.setZ((p4->getZ() + p1->getZ()) / 2);
		    p41.setNum(8);

		    cout << "p41" << endl;
		    cout << p41.getX() << endl;
		    cout << p41.getY() << endl;
		    cout << p41.getZ() << endl<<endl;
		    
		    
		    // p5 = le point entre p12 et p34 (le point au centre)

		    Vertex p5(0,0,0,0);
		    p5.setX((p12.getX() + p34.getX()) / 2); 
		    p5.setY((p12.getY() + p34.getY()) / 2); 
		    p5.setZ((p12.getZ() + p34.getZ()) / 2);
		    p5.setNum(9);

		    cout << "p5" << endl;
		    cout << p5.getX() << endl;
		    cout << p5.getY() << endl;
		    cout << p5.getZ() << endl<<endl;

		    // ON AJOUTE LES VERTEX A LA FORME DE DEPART

		    f->addVertex(p12);
		    f->addVertex(p23);
		    f->addVertex(p34);
		    f->addVertex(p41);
		    f->addVertex(p5);
		    
		    // Je suis obligé de redéfinir p1, ... , p4 sinon ils deviennent nuls  ?!?! (Et j'ai pas compris pourquoi...)
		    p1 = f->getVertex(sommets->at(0));
		    p2 = f->getVertex(sommets->at(1));
		    p3 = f->getVertex(sommets->at(2));
		    p4 = f->getVertex(sommets->at(3));

		    // LES 4 FACES CONSTRUITES A PARTIR DES NOUVEAUX POINTS

		    vector<int> s;

		    // face 1 (f1)
		    s.clear();
		    s.push_back(p1->getNum());
		    s.push_back(p12.getNum());
		    s.push_back(p5.getNum());
		    s.push_back(p41.getNum());

		    Face f1( 11 ); // A CHANGER ! CODE EN DUR :/ !
		    f1.setSommets( &s );

		    f->addFace(f1);


		    // face 2 (f2)
		    s.clear();
		    s.push_back(p12.getNum());
		    s.push_back(p2->getNum());
		    s.push_back(p23.getNum());
		    s.push_back(p5.getNum());

		    Face f2( 12 ); // Idem
		    f2.setSommets( &s );
		    f->addFace(f2);
		    
		    // face 3 (f3)
		    s.clear();
		    s.push_back(p5.getNum());
		    s.push_back(p23.getNum());
		    s.push_back(p3->getNum());
		    s.push_back(p34.getNum());

		    Face f3( 13 ); // Idem
		    f3.setSommets( &s );
		    f->addFace(f3);

		    // face 4 (f4)
		    s.clear();
		    s.push_back(p41.getNum());
		    s.push_back(p5.getNum());
		    s.push_back(p34.getNum());
		    s.push_back(p4->getNum());

		    Face f4( 14 ); // Idem
		    f4.setSommets( &s );
		    f->addFace(f4);

		    // ON SUPPRIME LA FACE

		    //f->removeFace(pFaces->at(i));



		}
		else {
		    cout << "La face ne semble pas avoir 4 sommets" << endl;
		    cout << "Nbr de sommets de la face" << sommets->size() << endl;
		}

	    }
	}

    }

}

int main(){


    Forme forme;



    vector<Vertex> v;
    Vertex v1( 1, -1, 1, 0 );// num x y z 
    v.push_back(v1);
    Vertex v2( 2, -2, 1, 0 );
    v.push_back(v2);
    Vertex v3( 3, -2, 2, 0 );
    v.push_back(v3);
    Vertex v4( 4, -1, 2, 0 );
    v.push_back(v4);

    vector<int> sommets;
    sommets.push_back(1);
    sommets.push_back(2);
    sommets.push_back(3);
    sommets.push_back(4);
    
    
    Face face( 1 );
    face.setSommets( &sommets );

    forme.setVertices( &v );
    forme.addFace( face );

    scission(&forme, 1);


    vector<Vertex> * v666;
    vector<Face> * f666;

    f666 = forme.getFaces();
    v666 = forme.getVertices();

    cout << endl << "AFFICHAGE DES VERTICES" << endl;

    for(int i=0; i<v666->size(); i++) {
	cout << "num : " << v666->at(i).getNum() << endl;
	cout <<"x "<< v666->at(i).getX() << endl;
	cout <<"y "<< v666->at(i).getY() << endl;
	cout <<"z "<< v666->at(i).getZ() << endl;
    }

    cout << endl << "AFFICHAGE DES FACES" << endl;

    for(int i=0; i<f666->size(); i++) {
	cout << "Numéro de la face " <<f666->at(i).getNum() << endl;
	vector<int> * s666;
	//s666->clear(); Déclenche une erreur de segmentation ? Surement normal j'ai pas regardé la doc à ce sujet.
	s666 = f666->at(i).getSommets();
	for(int i=0;i< s666->size();i++) {
	    cout<<s666->at(i)<<" ";
	}
	cout << endl;
    }




    /*
    Vertex a(1, 1, 1, 0 );
    Vertex b(2, 1, 4, 0);
    Vertex c(3, 4, 4, 0);
    Vertex d(4, 4, 1, 0);

    Vertex p1(5, 2, 2, 0);
    Vertex p2(6, 100, 2, 0);

    if(vertexInsidePolyg(a,b,c,d,p1)) {
	cout << "[!] P1 est dans le polygone [!]" << endl;
    }
    if(vertexInsidePolyg(a,b,c,d,p2)) {
	cout << "[!] P2 est dans le polygone [!]" << endl;
    }

    if(intersectionSegmentBoundingBox(p1,p2,a,b,c,d)) {
	cout << "Intersection avec la bounding box" << endl;
    }
    else {
	cout << "Pas intersection avec la bounding box" << endl;
    }
     */
}

