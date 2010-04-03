#include "forme.h"


Forme::Forme() {
	this->num = 0;
	this->vertices = vector<Vertex>();
	this->faces = vector<Face>();
}

Forme::Forme( int num) {
	this->num = num;
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

void Forme::addVertex(Vertex v){
	this->vertices.push_back(v);
}

void Forme::addFace(Face f){
	this->faces.push_back(f);
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
            v->findVoisins( f );
        }
    }
}

void Forme::draw() {
	//pour chaque face de cette forme
	for ( int i=0; i<this->faces.size(); i++ ) {
		glBegin( GL_POLYGON );
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
			glColor3ub( (int) ((y+4)*10)%255, 30, 30 );
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
