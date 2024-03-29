#include "forme.h"


Forme::Forme() {
	this->num = 0;
	this->vertices = vector<Vertex>();
	this->faces = vector<Face>();
	this->r=255;
	this->v=50;
	this->b=150;
}

Forme::Forme( int num) {
	this->num = num;
	this->r=255;
	this->v=153;
	this->b=0;
}

int Forme::getNum() {
	return this->num;
}

vector<Vertex> * Forme::getVertices(){
	return &(this->vertices);
}

vector<Face> * Forme::getFaces(){
	return &(this->faces);
}

Vertex * Forme::getVertex( int numVertex ) {
	for ( int i=0; i<this->vertices.size(); i++ ) {
		if ( this->vertices.at(i).getNum() == numVertex ) {
			return &this->vertices.at(i);
		}
	}
	return NULL;
}

void Forme::setNum( int n ) {
	this->num = n;
}

void Forme::setVertices( vector<Vertex> * vVertex ){
	this->vertices = (* vVertex);
}

void Forme::setFaces( vector<Face> * vFaces ){
	this->faces = (* vFaces);
}

void Forme::setCouleur( int r, int v, int b ) {
	this->r=r;
	this->v=v;
	this->b=b;
}

void Forme::addVertex(Vertex v){
	this->vertices.push_back(v);
}

void Forme::addFace(Face f){
	this->faces.push_back(f);
}

void Forme::findVoisinsVertex( Vertex * v, Face * f ) {
    vector<int> * sommets = f->getSommets();
    for ( int i=0; i<sommets->size(); i++ ) {
        //si je suis sur le sommet qui correspond au vertex
        if ( sommets->at(i) == v->getNum() ) {
            //j'ajoute le sommet précedent
            int p;
            //si i est le premier, il faut choisir le dernier
            if ( i == 0 )
                p = sommets->at(sommets->size()-1);
            else
                p = sommets->at(i-1);

            //si je n'ai pas deja ce voisin
            if ( !v->dejaVoisin( p ) ) {
                v->addVoisin( this->getVertex( p ) );
            }

            //j'ajoute le sommet suivant
            int s;
            //si i est le dernier, il faut choisir le premier
            if ( i == sommets->size()-1 )
                s = sommets->at(0);
            else
                s = sommets->at(i+1);

            //si je n'ai pas deja ce voisin
            if ( !v->dejaVoisin( s ) ) {
                v->addVoisin( this->getVertex( s ) );
            }
        }
    }
}

void Forme::generateGraph() {
    //pour chaque face de la forme
    for ( int i=0; i<this->getFaces()->size(); i++ ) {
        Face * f = &(this->getFaces()->at(i));
        //pour chaque sommet de la face
        for ( int j=0; j<f->getSommets()->size(); j++ ) {
            //on recupere le sommet correspondant
            Vertex * v = this->getVertex( f->getSommets()->at(j) );
            //on cherche ses voisins
            this->findVoisinsVertex( v, f );
        }
    }
}

void Forme::removeSommet( Vertex * v ) {
	v->isolation();
}

void Forme::parcoursGraph() {
	Vertex * v = NULL;
	int i=0;
	//on récupère un sommet non isolé pour commencer le parcours
	while ( v == NULL && i < this->vertices.size() ) {
		//si le sommet à au moins un voisin ( qu'il n'est pas isolé )
		if ( !this->vertices.at(i).estIsole() ) {
			v = &(this->vertices.at(i));
		}
		i++;
	}

	//si on en a trouver un
	if ( v != NULL ) {
		v->parcoursVoisins();
	}

	//une fois que le parcours a été fait, on remet tout à 0
	//pour pouvoir refaire un autre parcours
	for ( int i=0; i<this->vertices.size(); i++ ) {
		this->vertices.at(i).setVisite( 0 );
	}
}

void Forme::parcoursGraphDessiner() {
	Vertex * v = NULL;
	int i=0;
	//on récupère un sommet non isolé pour commencer le parcours
	while ( v == NULL && i < this->vertices.size() ) {
		//si le sommet à au moins un voisin ( qu'il n'est pas isolé )
		if ( !this->vertices.at(i).estIsole() ) {
			v = &(this->vertices.at(i));
		}
		i++;
	}

	//si on en a trouver un
	if ( v != NULL ) {
		v->parcoursVoisinsDessiner();
	}

	//une fois que le parcours a été fait, on remet tout à 0
	//pour pouvoir refaire un autre parcours
	for ( int i=0; i<this->vertices.size(); i++ ) {
		this->vertices.at(i).setVisite( 0 );
	}
}

void Forme::parcoursGraphMerging() {
	Vertex * v = NULL;
	int i=0;
	//on récupère un sommet non isolé pour commencer le parcours
	while ( v == NULL && i < this->vertices.size() ) {
		//si le sommet à au moins un voisin ( qu'il n'est pas isolé )
		if ( !this->vertices.at(i).estIsole() ) {
			v = &(this->vertices.at(i));
		}
		i++;
	}

	//si on en a trouver un
	if ( v != NULL ) {
		v->parcoursVoisinsMerging( 0 );
	}

	//une fois que le parcours a été fait, on remet tout à 0
	//pour pouvoir refaire un autre parcours
	for ( int i=0; i<this->vertices.size(); i++ ) {
		this->vertices.at(i).setVisite( 0 );
	}
}

Vertex * Forme::premierSommetValide() {
	Vertex * v = NULL;
	int i=0;
	//on récupère un sommet non isolé pour commencer le parcours
	while ( v == NULL && i < this->vertices.size() ) {
		//si le sommet à au moins un voisin ( qu'il n'est pas isolé )
		if ( !this->vertices.at(i).estIsole() ) {
			v = &(this->vertices.at(i));
		}
		i++;
	}

	return v;
}

/*void Forme::parcoursGraphSuppressionArete(vector <BoundingBox> liBB){
	
	Vertex * v = NULL;
	int i=0;
	//on récupère un sommet non isolé pour commencer le parcours
	while ( v == NULL && i < this->vertices.size() ) {
		//si le sommet à au moins un voisin ( qu'il n'est pas isolé )
		if ( !this->vertices.at(i).estIsole() ) {
			v = &(this->vertices.at(i));
		}
		i++;
	}

	//si on en a trouver un
	if ( v != NULL ) {
		v->parcoursVoisinsSuppressionArete(liBB);
	}

	//une fois que le parcours a été fait, on remet tout à 0
	//pour pouvoir refaire un autre parcours
	for ( int i=0; i<this->vertices.size(); i++ ) {
		this->vertices.at(i).setVisite( 0 );
	}
}*/

void Forme::draw() {
	//pour chaque face de cette forme
	for ( int i=0; i<this->faces.size(); i++ ) {
		glBegin( GL_TRIANGLE_FAN );
		//face courante
		vector<int> * f = this->faces.at(i).getSommets();
		//pour chaque vertex de cette face
		for ( int j=0; j<f->size(); j++ ) {
			int numVertex = f->at(j);
			Vertex * v = this->getVertex( numVertex );
			float x = v->getX();
			float y = v->getY();
			float z = v->getZ();
/*			cout << "( " << x << ", " << y << ", " << z << " )" << endl;*/
			//on crée un glVert
			glColor3ub( (y*20)+this->r, (y*20)+this->v, (y*20)+this->b );
			glVertex3d( x, y, z );

		}
		glEnd();
	}
}

string Forme::printGraph() {
    string str;
    for ( int i=0; i<this->getVertices()->size(); i++ ) {
        Vertex * v = &(this->getVertices()->at(i));
        str.append(v->printVoisins());
        str.append("\n");
    }

    return str;
}
