#pragma once


class Vector3D{
	public:
		double x,y,z;
		Vector3D(){
			x=y=z=0;
		}
		Vector3D(double x,double y,double z){
			this->x=x;
			this->y=y;
			this->z=z;
		}
		void set(double x,double y,double z){
			this->x=x;
			this->y=y;
			this->z=z;
		}
		//get length of vector
		double getNorm(){
			return sqrt( x*x+y*y+z*z );
		}
		static double getNorm(const Vector3D& a){
			return sqrt( a.x*a.x+a.y*a.y+a.z*a.z );
		}
		//get distance between vector a and vector b
		//get distance between vector a and vector b
		static double getDistance(const Vector3D& a, const Vector3D& b){
			return sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z) );
		}

		static double getAngle(const Vector3D& a, const Vector3D& b){
			Vector3D s;
			s.x=a.y*b.z-a.z*b.y;
			s.y=a.z*b.x-a.x*b.z;
			s.z=a.x*b.y-a.y*b.x;
			if(s.z<0)return -s.getNorm();
			else return s.getNorm();
		}
		
		static Vector3D normalize(Vector3D a){
				double n = a.getNorm();
				if(n==0)return a;
				Vector3D s;
				s.x+=a.x/n;
				s.y+=a.y/n;
				s.z+=a.z/n;
				return s;
		}

		//inner product
		static double inner_product(const Vector3D& a, const Vector3D& b){
			return a.x*b.x+a.y*b.y+a.z*b.z;
		}

		//cross product
		static Vector3D cross_product(const Vector3D& a, const Vector3D& b){
			Vector3D s;
			s.x=a.y*b.z-a.z*b.y;
			s.y=a.z*b.x-a.x*b.z;
			s.z=a.x*b.y-a.y*b.x;
			return s;
		}

		//rotation matrix
		Vector3D rot( Vector3D d, Vector3D n, double theta){
			Vector3D s;
			float cost=cos(theta),sint=sin(theta);
			s.x= ( n.x*n.x*(1-cost) + cost     )*d.x
				+ ( n.x*n.y*(1-cost) - sint*n.z )*d.y
				+ ( n.x*n.z*(1-cost) + sint*n.y )*d.z;

			s.y= ( n.y*n.x*(1-cost) + sint*n.z )*d.x
				+ ( n.y*n.y*(1-cost) + cost     )*d.y
				+ ( n.y*n.z*(1-cost) - sint*n.x )*d.z;

			s.z= ( n.z*n.x*(1-cost) - sint*n.y )*d.x
				+ ( n.z*n.y*(1-cost) + sint*n.x )*d.y
				+ ( n.z*n.z*(1-cost) + cost     )*d.z;
			return s;
		}

		Vector3D& operator +=(const Vector3D& a){
			x+=a.x;
			y+=a.y;
			z+=a.z;
			return *this;
		}

		Vector3D& operator -=(const Vector3D& a){
			x-=a.x;
			y-=a.y;
			z-=a.z;
			return *this;
		}

		Vector3D operator+(Vector3D a){
			a.x = x + a.x;
			a.y = y + a.y;
			a.z = z + a.z;
			return a;
		}
		Vector3D operator-(Vector3D a){
			a.x = x - a.x;
			a.y = y - a.y;
			a.z = z - a.z;
			return a;
		}
		friend Vector3D operator*(double a, Vector3D b){
			b.x *= a;
			b.y *= a;
			b.z *= a;
			return b;
		}


};
