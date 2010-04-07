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

void Vertex::findVoisins( Face * f, Forme * maForme ) {
    vector<int> * sommets = f->getSommets();
    for ( int i=0; i<sommets->size(); i++ ) {
        //si je suis sur le sommet qui me correspond
        if ( sommets->at(i) == this->getNum() ) {
            //j'ajoute le sommet précedent
            int p;
            //si i est le premier, il faut choisir le dernier
            if ( i == 0 )
                p = sommets->at(sommets->size()-1);
            else
                p = sommets->at(i-1);

            //si je n'ai pas deja ce voisin
            if ( !this->dejaVoisin( p ) ) {
                this->addVoisin( maForme->getVertex( p ) );
            }

            //j'ajoute le sommet suivant
            int s;
            //si i est le dernier, il faut choisir le premier
            if ( i == sommets->size()-1 )
                s = sommets->at(0);
            else
                s = sommets->at(i+1);

            //si je n'ai pas deja ce voisin
            if ( !this->dejaVoisin( s ) ) {
                this->addVoisin( maForme->getVertex( s ) );
            }
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
