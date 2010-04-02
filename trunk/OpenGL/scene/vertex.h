#ifndef VERTEX_H
#define VERTEX_H

class Vertex {
	private:
		int num;
		float x;
		float y;
		float z;

	public:
		Vertex();
		Vertex( int num, float x, float y, float z );

		int getNum();
		float getX();
		float getY();
		float getZ();

		void setNum(int n);
		void setX(float x);
		void setY(float y);
		void setZ(float z);
};

#endif
