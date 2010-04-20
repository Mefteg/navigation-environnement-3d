#include "vertex.h"

Vertex::Vertex() {
	this->num = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->visite = 0;
}

Vertex::Vertex( int num, float x, float y, float z ) {
	this->num = num;
	this->x = x;
	this->y = y;
	this->z = z;
	this->visite = 0;
}

int Vertex::getNum() {
	return this->num;
}

float Vertex::getX() {
	return this->x;
}

float Vertex::getY() {
	return this->y;
}

float Vertex::getZ() {
	return this->z;
}

vector<Vertex *> * Vertex::getVoisins() {
    return &(this->voisins);
}

int Vertex::getVisite() {
	return visite;
}

void Vertex::setNum( int n ) {
	this->num = n;
}

void Vertex::setX( float x ) {
	this->x = x;
}

void Vertex::setY( float y ) {
	this->y = y;
}

void Vertex::setZ( float z ) {
	this->z = z;
}

void Vertex::addVoisin( Vertex * v ) {
    this->voisins.push_back( v );
}

void Vertex::setVisite( int i ) {
	this->visite = i;
}

void Vertex::removeVoisin( int n ) {
    int i=0;
    int continuer=1;
    //tant qu'on a rien trouvé et qu'on n'a pas vu tous les voisins
    while ( i < this->getVoisins()->size() && continuer ) {
        //si c'est le voisin à supprimer
        if ( this->getVoisins()->at(i)->getNum() == n ) {
            //on le supprime
            this->getVoisins()->erase(this->getVoisins()->begin()+i);
            //et on arrete la recherche
            continuer=0;
        }
		i++;
    }
}

bool Vertex::dejaVoisin( int p ) {
    for ( int i=0; i<this->getVoisins()->size(); i++ ) {
        if ( this->getVoisins()->at(i)->getNum() == p )
            return true;
    }

    return false;
}

//parcours tous les voisins de maniere recursive
void Vertex::parcoursVoisins() {
	//si le sommet n'a pas deja été visité
	if ( !this->getVisite() ) {
		this->setVisite( 1 );
		for ( int i=0; i<this->voisins.size(); i++ ) {
			this->voisins.at(i)->parcoursVoisins();
		}
	}
}

//parcours tous les voisins de maniere recursive
void Vertex::parcoursVoisinsDessiner() {
	//si le sommet n'a pas deja été visité
	if ( !this->getVisite() ) {
		float x = this->getX();
		float y = this->getY();
		float z = this->getZ();
		glVertex3d( x, y, z );
		glEnd();
		this->setVisite( 1 );
		this->draw( 255, 0, 0 );
		for ( int i=0; i<this->voisins.size(); i++ ) {
			glLineWidth( 2.0 );
			glBegin( GL_LINES );
			glColor3ub( 0, 255, 0 );
			glVertex3d( x, y, z );
			this->voisins.at(i)->parcoursVoisinsDessiner();
		}
	}
	else {
		glEnd();
	}
}

//retourne 1 si le sommet est isolé, 0 sinon
int Vertex::estIsole() {
	if ( this->getVoisins()->empty() )
		return 1;
	else
		return 0;
}

string Vertex::printVoisins() {
    string str;
    cout << "nb de voisins: " << this->getVoisins()->size() << endl;
    for ( int i=0; i<this->getVoisins()->size(); i++ ) {
        ostringstream sin;
        sin << this->getVoisins()->at(i)->getNum();
        str.append(sin.str());
        str.append(" ");
    }

    return str;
}

void Vertex::draw( int r, int g, int b ) {
	float x = this->getX();
	float y = this->getY();
	float z = this->getZ();
	glPointSize( 5.0 );
	//on crée un glVert
	glBegin( GL_POINTS );
	glColor3ub( r, g, b );
	glVertex3d( x, y, z );
	glEnd();
}