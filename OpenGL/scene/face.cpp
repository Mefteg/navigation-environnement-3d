#include "face.h"

Face::Face() {
	this->num = 0;
	this->sommets = vector<int>();
}

Face::Face( int n) {
	this->num = n;
}

int Face::getNum() {
	return this->num;
}

vector<int> * Face::getSommets(){
	return &(this->sommets);
}

void Face::setNum( int n ) {
	this->num = n;
}

void Face::setSommets( vector<int> * vSommets ){
	this->sommets = (* vSommets);
}

void Face::addSommet( int num_vertex  ){
	this->sommets.push_back(num_vertex);
}

void Face::removeSommet( int num_vertex ){

	int i=0;
	while(this->sommets[i]!=num_vertex){
		i++;
	}
	this->sommets.erase(sommets.begin()+i);

}




