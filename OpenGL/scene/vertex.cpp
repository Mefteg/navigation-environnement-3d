#include "vertex.h"

Vertex::Vertex() {
	this->num = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vertex::Vertex( int num, float x, float y, float z ) {
	this->num = num;
	this->x = x;
	this->y = y;
	this->z = z;
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
    }
}

bool Vertex::dejaVoisin( int p ) {
    for ( int i=0; i<this->getVoisins()->size(); i++ ) {
        if ( this->getVoisins()->at(i)->getNum() == p )
            return true;
    }

    return false;
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
