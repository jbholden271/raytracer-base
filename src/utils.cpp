#include <cmath.h>

#define PI 3.14159265359
#define INF 1e8

template <typename T>
class Vec3 {
public:
	// data members
	T x;
	T y;
	T z:
	// constructors
	Vec3() {
		x = T(0);
		y = T(0);
		z = T(0);
	}
	Vec3(T xp, T yp, T, zp) {
		x = xp;
		y = yp;
		z = zp;
	}
	// operator overloads
	Vec3<T> operator + (T s) { return new Vec3<T>(x+s, y+s, z+s); }
	Vec3<T> operator + (Vec3<T> v) { return new Vec3<T>(x+v.x, y+v.y, z+v.z); }
	Vec3<T> operator - () { return new Vec3<T>(-x, -y, -z); }
	Vec3<T> operator - (T s) { return new Vec3<T>(x-s, y-s, z-s); }
	Vec3<T> operator - (Vec3<T> v) { return new Vec3<T>(x-v.x, y-v.y, z-v.z); }
	Vec3<T> operator * (T s) { return new Vec3<T>(x*s, y*s, z*s); }
	Vec3<T> operator * (Vec3<T> v) { return new Vec3<T>(x*v.x, y*v.y, z*v.z); }
	Vec3<T> operator / (T s) { return new Vec3<T>(x/s, y/s, z/s); }
	Vec3<T> operator / (Vec3<T> v) { return new Vec3<T>(x/v.x, y/v.y, z/v.z); }
	Vec3<T> cross(Vec3<T> v) { return new Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y-v.x); }
	T dot(Vec3<T> v) { return x*v.x + y*v.y + z*v.z; }
	T len2() { return x*x + y*y + z*z; }
	void normalize() { 
		T nSca = sqrt(len2());
		x = x / nSca;
		y = y / nSca;
		z = z / nSca;
	}
	void add(Vec3<T> v1) {
		x += v1.x;
		y += v1.y;
		z += v1.z;
	}
}

template <typename T>
class Material {
public:
	// data members
	Vec3<T> ka;  // ambient reflectance  (color)
	Vec3<T> kd;  // diffuse reflectance  (color)
	Vec3<T> ks:  // specular reflectance (color)
	Vec3<T> kr;  // mirror reflectance   (color)
	Vec3<T> kt;  // transparency         (color)
	T p;         // specular shininess   (power)
	T ior;       // index of refraction  (transparent material)
	// constructors (determines qualities of a material)
	Material() {
		ka = NULL;
		kd = NULL;
		ks = NULL;
		kr = NULL;
		kt = NULL;
		p = 0;
		ior = 0;
	}

	void DiffuseMaterial(Vec3<T> kap, Vec3<T> kdp) {
		ka = kap;
		kd = kdp;
	}
	void PhongMaterial(Vec3<T> kap, Vec3<T> kdp, Vec3<T> ksp, T pp) {
		ka = kap;
		kd = kdp;
		ks = ksp;
		p = pp;
	}
	void MirrorMaterial(Vec3<T> krp) {
		kr = krp;
	}
	void GlassMaterial(Vec3<T> krp, Vec3<T> ktp, T iorp) {
		kr = krp;
		kt = ktp;
		ior = iorp;
	}
}

template <typename T>
class BoundingBox {
public:
	// data members
	Vec3<T> min;
	Vec3<T> max;
	// constructors
	Vec3() {
		min = new Vec3<T>(INF, INF, INF);
		max = new Vec3<T>(-INF, -INF, -INF);
	}
	Vec3(Vec3<T> minp, Vec3<T> maxp) {
		min = minp;
		max = maxp;
	}
	bool containsPoint(Vec3<T> point) {
		return (min.x <= point.x) && (min.y <= point.y) && (min.z <= point.z) && 
							(max.x >= point.x) && (max.y >= point.y) && (max.z >= point.z);
	}
	bool isEmpty() {
		return !((max.x >= min.x) && (max.y >= min.y) && (max.z >= min.z));
	}
	void expandByPoint(Vec3<T> point) {
		min.x = (min.x < point.x) ? min.x : point.x;
		min.y = (min.y < point.y) ? min.y : point.y;
		min.z = (min.z < point.z) ? min.z : point.z;
		max.x = (max.x > point.x) ? max.x : point.x;
		max.y = (max.y > point.y) ? max.y : point.y;
		max.z = (max.z > point.z) ? max.z : point.z;
	}
	void set(Vec3<T> newMin, Vec3<T> newMax) {
		min = newMin;
		max = newMax;
	}
	void boxUnion(BoundingBox boxp) {
		min.x = (min.x <= boxp.min.x) ? min.x : boxp.min.x;
		min.y = (min.y <= boxp.min.y) ? min.y : boxp.min.y;
		min.z = (min.z <= boxp.min.z) ? min.z : boxp.min.z;
		max.x = (max.x >= boxp.max.x) ? max.x : boxp.max.x;
		max.y = (max.y >= boxp.max.y) ? max.y : boxp.max.y;
		max.z = (max.z >= boxp.max.z) ? max.z : boxp.max.z;
	}
}