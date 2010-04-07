#ifndef FACE_H
#define FACE_H

#include <vector>
#include "vertex.h"

using namespace std;

class Face {
	private:
		int num;
		vector<int> sommets;
		
	public:
		Face();
		Face( int n );

		int getNum();
		vector<int> * getSommets();

		void setNum( int n );
		void setSommets( vector<int> * vSommets );

		void addSommet( int num_vertex );

		void removeSommet( int num_vertex );

		//string toString();

};

#endif
