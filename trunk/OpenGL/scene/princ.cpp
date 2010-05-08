#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "forme.h"
#include "vertex.h"
#include "face.h"
#include "BoundingBox.h"

using namespace std;

// Taille de la fenêtre
const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

// Position de la caméra
int xCam=5;
int yCam=5;

/**
 * Parse un fichier .obj passé en paramètre (selon son chemin)
 *	
 *	@param vFormes : vector<forme> * vFormes : permet de stocker les formes détectées 
 * */

int parser( string chemin, vector<Forme> * vFormes ) {

    int pos; 
    string l; 
    vector<int> vFaces;
    int nv_forme=1;
    Forme forme( 1 ); // Forme numéro 1
    int cpt_vertex=1; // Sert à compter le nombre de vertex trouvé

    //On ouvre le fichier
    ifstream fichier( chemin.c_str(), ios::in );

    //Si l'ouverture du fichier s'est bien passée
    if ( fichier ) {
	string ligne;
	//Tant qu'on est pas arrivé à la fin du fichier
	while ( !fichier.eof() ) {
	    //On récupère toute la ligne
	    getline( fichier, ligne );
	    //On teste la première lettre de la ligne
	    switch ( ligne[0] ) {
		//S'il s'agit d'un vertex
		case 'v':
		    {
			//S'il s'agit d'une nouvelle forme
			if ( nv_forme == 0 ) {
				cout << "Nouvelle Forme" << endl;
			    //On enregistre la dernière forme
			    vFormes->push_back( forme );
			    //On va passer à la nouvelle forme
			    /*  cout << "Nouvelle forme" << endl;*/
			    forme = Forme( 1 );
			    //On signale qu'on traite une nouvelle forme
			    nv_forme = 1;
			}

			/*  int num_vertice=obj.vertices.size();*/

			//On récupère la position du premier espace
			pos = ligne.find_first_of(' ');
			ligne = ligne.substr( pos+1, ligne.length() );
			//On récupère la position du x
			pos = ligne.find_first_of(' '); 
			string l = ligne.substr( 0, pos );
			float x = atof(l.c_str());
			ligne = ligne.substr( pos+1, ligne.length() );
			//On récupère la position du y
			pos = ligne.find_first_of(' ');
			l = ligne.substr( 0, pos );
			float y = atof(l.c_str());
			ligne = ligne.substr( pos+1, ligne.length() );
			//Ce qu'il reste ( c'est le z )
			float z = atof(ligne.c_str());

			Vertex v( cpt_vertex, x, y, z );
			cpt_vertex++;

			forme.addVertex( v );
			/*  cout << "vertex = ( " << v.coord[0] << ", " << v.coord[1] << ", " << v.coord[2] << " )" << endl;*/
			break;
		    }


		    //S'il s'agit d'une face
		case 'f':
		    {
			//Une fois les faces terminées, on passera à une nouvelle forme
			nv_forme = 0;

			//On ajoute une face
			Face f( 1 );

			//On enleve le "f "
			pos = ligne.find_first_of(' ');
			ligne = ligne.substr( pos+1, ligne.length() );
			pos = ligne.find_first_of(' ');
			//Tant qu'il y a des espaces
			while ( pos > 0 ) {
			    //On recupère le debut de la ligne jusqu'au premier espace
			    string l = ligne.substr( 0, pos ); // on redéfinit l ?
			    //On transforme le résultat en flottant
			    f.addSommet( atoi(l.c_str()) );
			    //On découpe la ligne
			    ligne = ligne.substr( pos+1, ligne.length() );
			    //Et je cherche l'emplacement du premier espace
			    pos = ligne.find_first_of(' ');
			}
			//Ce qu'il reste
			f.addSommet( atoi(ligne.c_str()) );

			forme.addFace( f );
			break;
		    }
		    //Autrement
		default:
		    {
			break;
		    }
	    }
	}

	//on enregistre la dernière forme
	vFormes->push_back( forme );

	//on ferme le fichier
	fichier.close();
	return 1;
    }
    //sinon
    else {
	return 0;
    }
}

int detecterSol( vector<Forme> * vFormes ) {
	int taille=0;
	int max=0;
	for ( int i=0; i<vFormes->size(); i++ ) {
		if ( vFormes->at(i).getVertices()->size() > taille ) {
			taille = vFormes->at(i).getVertices()->size();
			max = i;
		}
	}

	return max;
}

//sort le sol du vecteur de formes vFormes et le retourne
Forme isolerSol( vector<Forme> * vFormes ) {
	int psol = detecterSol( vFormes );

	Forme sol = vFormes->at( psol );

	vFormes->erase( vFormes->begin()+psol );

	return sol;
}

/**
 * Dessine une scène précédemment parsée
 *
 * On dessine pour chaque forme, chacuns de ses points
 * On décide du mode de dessin (par exemple GL_LINE_LOOP)
 */
void dessinerScene( vector<Forme> * vFormes ) {
    //Pour chaque forme
    for ( int i=0; i<vFormes->size(); i++ ) {
		vFormes->at(i).draw();
    }
}

/**
 * Fonction pour tout initialiser correctement et utiliser la fenetre 
 * */
void dessiner( Forme * sol, vector<Forme> * vFormes ) {

    //efface le tampon d'affichage ( ? )
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    //le y définit la verticale
    gluLookAt(xCam,yCam,5,0,0,0,0,1,0);

    sol->draw();
    sol->parcoursGraphDessiner();

    //On s'assure que toutes les commandes OpenGL ont été exécutées
    glFlush();
    //Mise à jour de l'écran
    SDL_GL_SwapBuffers();
}

/*void dessinerForme( Forme forme ) {

//efface le tampon d'affichage ( ? )
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

glMatrixMode( GL_MODELVIEW );
glLoadIdentity( );

//le y définit la verticale
gluLookAt(xCam,yCam,5,0,0,0,0,1,0);

forme.draw();

//on s'assure que toutes les commandes OpenGL ont été exécutées
glFlush();
//mise à jour de l'écran
SDL_GL_SwapBuffers();
}*/

int main(int argc, char *argv[]){
    //On vérifie qu'il y est une carte à charger
    if ( argv[1] == NULL ) {
	cout << "xx Erreur: Il n'y a pas de carte à charger" << endl;
	return -1;
    }

    //On lance SDL
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    //On affiche un titre et une icone dans la Fenetre
    SDL_WM_SetCaption("HeadCrabs ~ TER", NULL);
    SDL_WM_SetIcon(SDL_LoadBMP("./img/headcrabs2.bmp"), NULL);

    //On fixe la taille de la Fenetre et indique le rendu openGL
    SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL);
    //On indique le rendu openGL
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 70, (double) SCREEN_WIDTH/SCREEN_HEIGHT, 1, 1000 );
    glEnable(GL_DEPTH_TEST);

    /* SDL_EnableKeyRepeat(10,10);*/

    // On appelle le parser :
    vector<Forme> vFormes;
	//On parse la scène 3D
    parser( argv[1], &vFormes );
	//On isole le sol des autres formes
	Forme sol = isolerSol( &vFormes );

    // On suppose que la première forme trouvé est le sol, comme le sol = notre maillage => on créé le graphe à partir du sol. 
    sol.generateGraph();
    sol.parcoursGraphMerging();

    // On genere les bouding box
    vector <BoundingBox> listeBoundingBox = formeToBoundingBox(vector<Forme>(vFormes.begin() + 1, vFormes.end()));

    bool continuer = true;
    //Les évènements SDL
    SDL_Event event;


    while (continuer)
    {
	dessiner( &sol, &vFormes );
	/* dessinerForme( forme );*/

	//Attente d'évènement méthode bloquante
	SDL_WaitEvent(&event);	
	switch(event.type) {
	    // Permet de fermer la fenêtre
	    case SDL_QUIT:
		continuer = false;
		break;

		// Gestion de la caméra avec la touches directionnelles 
	    case SDL_KEYDOWN:
		switch ( event.key.keysym.sym ) {
		    case SDLK_UP:
			yCam++;
			break;
		    case SDLK_DOWN:
			yCam--;
			break;
		    case SDLK_RIGHT:
			xCam++;
			break;
		    case SDLK_LEFT:
			xCam--;
			break;
		}
		break;
	}

    }

    SDL_Quit();

    return EXIT_SUCCESS;
}
