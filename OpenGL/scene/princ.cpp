#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "forme.h"
#include "BoundingBox.h"

using namespace std;

//VARIABLES GLOBALES

// Taille de la fenêtre
int SCREEN_WIDTH=640;
int SCREEN_HEIGHT=480;

// Position de la caméra
int xCam=5;
int yCam=5;

int angleSceneY=0;
int angleSceneZ=10;
int zoomScene=70;

//savoir o� se situe le personnage
Vertex * vertexPerso;
vector<Forme> personnage;


//savoir s'il faut afficher les formes ou non
int afficherFormes=0;
//savoir s'il faut afficher le graphe ou non
int afficherGraphe=0;

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
				case 'v':{
							 if ( ligne[1] != 't' ) {
					//S'il s'agit d'une nouvelle forme
					if ( nv_forme == 0 ) {
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
							 }
					break;
				}


				//S'il s'agit d'une face
				case 'f':{
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
				default:{
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

void dessinerPerso( Forme * sol ) {
	float x = vertexPerso->getX();
	float y = vertexPerso->getY();
	float z = vertexPerso->getZ();
	glPushMatrix();
	glTranslated( x, y, z );
	//si aucun personnage n'a �t� charg�
	if ( personnage.empty() ) {
		glLineWidth( 5.0 );
		glBegin( GL_LINES );
		glPointSize( 5 );
		glColor3ub( 102, 51, 102 );
		glVertex3i( 0, 0, 0 );
		glVertex3i( 0, 1, 0 );
		glEnd();
	}
	else {
		dessinerScene( &personnage );
	}
	glPopMatrix();
}

void changerVertexPerso() {
	vector<Vertex *> * voisins = vertexPerso->getVoisins();
	//on change le vertex courant du personnage
	int next=0;
	int aux=1000;
	int pas=10;
	for ( int i=0; i<voisins->size(); i++ ) {
		int p = voisins->at(i)->poidsParcours( pas, vertexPerso->getNum() );
		if ( p <= aux ) {
			next = i;
			aux = p;
		}
	}
	vertexPerso = vertexPerso->getVoisins()->at(next);
	vertexPerso->setPoids( vertexPerso->getPoids()+1 );
}


/**
 * Fonction pour tout initialiser correctement et utiliser la fenetre 
 * */
void dessiner( Forme * sol, vector<Forme> * vFormes ) {

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( zoomScene, (double) SCREEN_WIDTH/SCREEN_HEIGHT, 1, 1000 );

	glEnable(GL_DEPTH_TEST);

	//efface le tampon d'affichage ( ? )
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	//le y définit la verticale
/*	gluLookAt( zoomScene, 0, 0, 0, 0, 0, 0, 1, 0 );*/
	gluLookAt( angleSceneZ, 10, angleSceneZ, 0, 0, 0, 0, 1, 0 );

	glPushMatrix();

	glRotated( angleSceneY, 0, 1, 0 );
/*	glRotated( angleSceneZ, 0, 0, 1 );*/
/*	glScalef( zoomScene, zoomScene, zoomScene );*/

	sol->draw();
	dessinerPerso( sol );

	if ( afficherFormes ) {
		dessinerScene(vFormes);
	}

	if ( afficherGraphe ) {
		sol->parcoursGraphDessiner();
	}

	glPopMatrix();

	//On s'assure que toutes les commandes OpenGL ont été exécutées
	glFlush();
	//Mise à jour de l'écran
	SDL_GL_SwapBuffers();
}

void detectionSommetsInvalides( Forme * sol, vector<BoundingBox> * listeBoundingBox, vector<Forme> * vFormes, float temps ) {
	vector<Vertex> * vertexDuSol = sol->getVertices();
	for(int i = vertexDuSol->size(); i > 0; i--){
		if(vertexInsideBoundingBox(*(listeBoundingBox), vertexDuSol->at(i - 1))){
			// suppression
			vertexDuSol->at(i - 1).setRVB(0, 0, 250);
			if ( temps > 0 ) {
				dessiner( sol, vFormes );
			}
			vertexDuSol->at(i - 1).isolation();
			sleep( temps );
		}
	}
}


void detectionAretesInvalides( Forme * sol, vector<BoundingBox> * listeBoundingBox, vector<Forme> * vFormes ) {
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		PARCOURS POUR SUPPRIMER LES ARETES QUI COUPENT UNE BOUNDINGBOX
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	vector<Vertex> * vertexDuSol = sol->getVertices();

	Vertex * v = NULL;
	int i = vertexDuSol->size();
	//on recupere un sommet non isole pour commencer le parcours
	while(i > 0){
		i--;
		//si le sommet a� au moins un voisin (qu'il n'est pas isolee)
		if ( !vertexDuSol->at(i).estIsole() ) {
			v = &(vertexDuSol->at(i));
			v->setVisite(1);

			// On est face a un vertex a eplorer
			for(int j = 0; j < v->getVoisins()->size(); j++){
				if(!v->getVoisins()->at(j)->estIsole()){
					// si il est pas isol� on teste l'arte entre les deux
					if(segmentIntersectBoundingBox(*(listeBoundingBox),  *v, *(v->getVoisins()->at(j)))){
						// on supprime l'arete
						v->getVoisins()->at(j)->removeVoisin(v->getNum());
						v->removeDirectVoisin(j);
					}
				}
			}
		}
	}

	//une fois que le parcours a ete fait, on remet tout a� 0
	//pour pouvoir refaire un autre parcours
	for(int i = 0; i < vertexDuSol->size(); i++){
		vertexDuSol->at(i).setVisite(0);
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
}

int main(int argc, char *argv[]){
	//On vérifie qu'il y est une carte à charger
	if ( argv[1] == NULL ) {
		cout << "xx Erreur: Il n'y a pas de carte �charger" << endl;
		return -1;
	}

	//On vérifie qu'il y est un personnage � charger
	if ( argv[2] == NULL ) {
		cout << "xx Erreur: Il n'y a pas de personnage � charger" << endl;
	}
	else {
		parser( argv[2], &personnage );
		for ( int i=0; i<personnage.size(); i++ ) {
			personnage.at(i).setCouleur( 153, 52, 153 );
		}
	}

	//On lance SDL
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	//On affiche un titre et une icone dans la Fenetre
	SDL_WM_SetCaption("HeadCrabs ~ TER", NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("./img/headcrabs2.bmp"), NULL);

	//On fixe la taille de la Fenetre et indique le rendu openGL
	SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL | SDL_RESIZABLE);

	SDL_EnableKeyRepeat(10,10);

	// On appelle le parser :
	vector<Forme> vFormes;
	//On parse la scène 3D
	parser( argv[1], &vFormes );
	//On isole le sol des autres formes
	Forme sol = isolerSol( &vFormes );
	sol.setCouleur( 200, 200, 200 );

	//zero pour l'instant, on prendra le premier point non isol� plus tard :)
	vertexPerso = &(sol.getVertices()->at(0));
	vertexPerso->setPoids( vertexPerso->getPoids()+1 );

	// On suppose que la première forme trouvé est le sol, comme le sol = notre maillage => on créé le graphe à partir du sol. 
	sol.generateGraph();

	// On genere les bouding box
	vector <BoundingBox> listeBoundingBox = formeToBoundingBox(vFormes);
	
	bool continuer = true;
	//Les évènements SDL
	SDL_Event event;

	int evenementPertinent = 1;
	while (continuer){
		if ( evenementPertinent ) {
			dessiner( &sol, &vFormes );
			evenementPertinent = 0;
		}
		/* dessinerForme( forme );*/

		//Attente d'évènement méthode bloquante
		SDL_WaitEvent(&event);	
		switch(event.type) {
			// Permet de fermer la fenêtre
			case SDL_QUIT:
				continuer = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch ( event.button.button ) {
					case SDL_BUTTON_WHEELUP:
						zoomScene -= 2;
						break;

					case SDL_BUTTON_WHEELDOWN:
						zoomScene += 2;
						break;

					default:
						break;
			}
			evenementPertinent = 1;
			break;

			// Gestion de la caméra avec la touches directionnelles 
			case SDL_KEYDOWN:
				switch ( event.key.keysym.sym ) {
					case SDLK_UP:
						angleSceneZ -= 2;
						break;
					
					case SDLK_DOWN:
						angleSceneZ += 2;
						break;
					
					case SDLK_RIGHT:
						angleSceneY += 4;
					break;
					
					case SDLK_LEFT:
						angleSceneY -= 4;
						break;

					case SDLK_RETURN:
						//on fait avancer le perso
						changerVertexPerso();
						break;

					case SDLK_m:
						//simplification du graphe -> Merging
						sol.parcoursGraphMerging();
						break;

					case SDLK_c:
						if ( afficherFormes ) {
							afficherFormes = 0;
						}
						else {
							afficherFormes = 1;
						}
						break;

					case SDLK_t:
						if ( afficherGraphe ) {
							afficherGraphe = 0;
						}
						else {
							afficherGraphe = 1;
						}
						break;

					case SDLK_d:
						detectionSommetsInvalides( &sol, &listeBoundingBox, &vFormes, 0.1 );
						vertexPerso = sol.premierSommetValide();
						break;

					case SDLK_a:
						detectionSommetsInvalides( &sol, &listeBoundingBox, &vFormes, 0.0 );
						vertexPerso = sol.premierSommetValide();
						break;

					case SDLK_z:
						detectionAretesInvalides( &sol, &listeBoundingBox, &vFormes );
						vertexPerso = sol.premierSommetValide();
						break;

					case SDLK_g:
						sol.generateGraph();
						break;

					case SDLK_h:
						cout << endl << " ~~~~~ HELP ~~~~~" << endl;
						cout << "D�tecter les points invalides ( avec suivi visuel ): d" << endl;
						cout << "D�tecter les points invalides ( sans suivi visuel ): a" << endl;
						cout << "Simplifier le graphe ( Merging ): m" << endl;
						cout << "Afficher/Masquer les formes: c" << endl;
						cout << "Afficher/Masquer le graphe: t" << endl;
						cout << "Bouger la cam�ra: touches directionnelles" << endl;
						cout << endl;
						break;
				}
				evenementPertinent = 1;
			break;

			case SDL_VIDEORESIZE:
			SCREEN_WIDTH = event.resize.w; // <- largeur
			SCREEN_HEIGHT = event.resize.h; // <- hauteur
			SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL | SDL_RESIZABLE);
			glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
			gluPerspective( 70, (double) SCREEN_WIDTH/SCREEN_HEIGHT, 1, 1000 );

			evenementPertinent = 1;
			break;
		}

	}

	SDL_Quit();

	return EXIT_SUCCESS;
}
