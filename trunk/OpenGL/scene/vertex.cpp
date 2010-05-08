#include "vertex.h"

Vertex::Vertex() {
	this->num = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->r = 255;
	this->v = 0;
	this->b = 0;
	this->visite = 0;
}

Vertex::Vertex( int num, float x, float y, float z ) {
	this->num = num;
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = 255;
	this->v = 0;
	this->b = 0;
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

void Vertex::setVisite( int i ) {
	this->visite = i;
}

void Vertex::setRVB(int cr, int cv, int cb){
	r = cr;
	v = cv;
	b = cb;
}

int Vertex::nbVoisins() {
	return this->getVoisins()->size();
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

//Le vertex courant s'isole, cad qu'il indique à ses voisins qu'il
//s'isole puis detruit sa liste de voisins
void Vertex::isolation() {
	//je dis à mes voisins de me supprimer
	for ( int i=0; i<this->getVoisins()->size(); i++ ) {
		this->getVoisins()->at(i)->removeVoisin(this->getNum());
	}
	//puis je vide ma liste de voisins
	this->getVoisins()->clear();
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
		float x = this->getX();
		float y = this->getY();
		float z = this->getZ();
		glVertex3d( x, y, z );
		glEnd();
	//si le sommet n'a pas deja été visité
	if ( !this->getVisite() ) {

		this->setVisite( 1 );
		this->draw();
		for ( int i=0; i<this->voisins.size(); i++ ) {
			glLineWidth( 2.0 );
			glBegin( GL_LINES );
			glColor3ub( 0, 255, 0 );
			glVertex3d( x, y, z );
			this->voisins.at(i)->parcoursVoisinsDessiner();
		}
	}
}

void Vertex::parcoursVoisinsMerging( int profondeur ) {
	//si le sommet n'a pas deja été visité
	if ( !this->getVisite() && !this->estIsole() ) {

		this->setVisite( 1 );

		if ( this->b+profondeur > 255 ) {
			this->b = 255;
			this->v = (this->v+profondeur+100)%255;
		}
		else {
			this->b = (this->b+profondeur)%255;
		}


	cout << this->r << " ";
	cout << this->v << " ";
	cout << this->b << " ";
	cout << endl;

		//si j'ai 4 voisins
		if ( this->nbVoisins() == 4 ) {
			int cpt=0;
			//je compte le nombre de mes voisins qui ont au moins 3 voisins
			for ( int i=0; i<this->getVoisins()->size(); i++ ) {
				if ( this->getVoisins()->at(i)->nbVoisins() >= 3 ) {
					cpt++;
				}
			}
			//si mes voisins ont tous au moins 3 voisins
			if ( cpt == 4 ) {
				//je m'isole
				for ( int i=0; i<this->getVoisins()->size(); i++ ) {
					this->getVoisins()->at(i)->removeVoisin( this->getNum() );
				}
			}

			//je fais poursuivre le parcours
			for ( int i=0; i<this->getVoisins()->size(); i++ ) {
				getVoisins()->at(i)->parcoursVoisinsMerging( profondeur+1 );
			}

			if ( cpt == 4 ) {
				this->getVoisins()->clear();
			}
		}
		else {
			//je fais poursuivre le parcours
			for ( int i=0; i<this->getVoisins()->size(); i++ ) {
				getVoisins()->at(i)->parcoursVoisinsMerging( profondeur+1 );
			}
		}
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

void Vertex::draw() {
	float x = this->getX();
	float y = this->getY();
	float z = this->getZ();
	glPointSize( 5.0 );
	//on crée un glVert
	glBegin( GL_POINTS );
	glColor3ub( this->r, this->v, this->b );
	glVertex3d( x, y, z );
	glEnd();
}
