#include "Include.hpp"
#include "F3.hpp"

class Surface{
private:
	static int count;
	int id;
	F3 s1,s2,s3,s4;
	F3 n;
	F3 color;
public:
	Surface();
	~Surface();
	void set(F3 a1,F3 a2,F3 a3,F3 a4);
	void drow();
};