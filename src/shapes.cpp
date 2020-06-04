
#include <cmath.h>
#include "utils.h"

typedef float T;
typedef Vec3<T> Vec;
typedef Material<T> Mat;
typedef BoundingBox<T> BB;


class Intersection {
public:
	T t;
	Vec position, normal;
	Mat material;
	Intersection(T tp, Vec pp, Vec np, Mat mp) {
		t = tp;
		position = pp;
		normal = np;
		material = mp;
	}
	void set(isect) {
		t = isect.t;
		position = isect.position;
		normal = isect.normal;
		material = isect.material;
	}
}

class Ray {
public:
	Vec origin, direction;
	Ray(Vec op, Vec dp) {
		origin = op;
		direction = dp;
		direction.normalize();
	}
	pointAt(T t) {
		return direction * t + origin;
	}
}

class Shape {
public: 
	virtual Intersection intersect() = 0;
	virtual BB boundingBox() = 0;
}

class Plane: public Shape {
public:
	Vec P0, n;
	Mat material;
	Plane(Vec P0p, Vec np, Mat mp) {
		P0 = P0p;
		n = np;
		n.normalize();
		material = mp;
	}
	Intersection intersect(Ray ray, T tmin, T tmax) {
		T denom = ray.direction.dot(n);
		if(denom == 0) { return null; } // if the dot product is zero, the ray is parallel to the plane
		Vec t = (P0-ray.origin)/denom;
		if(t < tmin || t > tmax) { return null; }
		Vec iPoint = ray.pointAt(t);
		return new Intersection(t, iPoint, n, mat);
	};

}


class Sphere: public Shape {
public:
	Vec c;
	T r, r2;
	Mat material;
	Sphere(Vec op, T rp, Mat mp) {
		c = cp;
		r = rp;
		r2 = r * r;
		material = mp;
	}
	Intersection intersect(Ray ray, T tmin, T tmax) {
		T valB = (ray.origin - c).dot(ray.direction * 2);
		T valC = (ray.origin - c).len2()-r2;
		T delta = (valB * valB) - (valC * 4);
		if(delta >= 0) {
			T sqrtDelta = sqrt(delta);
			T intPts[2] = [(-valB - sqrtDelta)/2, (-valB + sqrtDelta)/2];
			bool tInRange[2] = {intPts[0] >= tmin && intPts[0] <= tmax, intPts[1] >= tmin && intPts[1] <= tmax};
			T t = 0;
			if(!tInRange[0] && !tInRange[1]) {
				return null;
			} else if (tInRange[0] && tInRange[1]) {
				t = (intPts[0] < intPts[1]) ? intPts[0] : intPts[1];
			} else {
				t = tInRange[0] ? intPts[0] : intPts[1];
			}
			Vec iPoint = ray.pointAt(t);
			return temp = new Intersection(t, iPoint, (iPoint-c).normalize(), mat);
		}
	}
}


class Triangle: public Shape {
public:
	Vec P0, P1, P2;
	Mat material;
	Triangle(Vec P0p, Vec P1p, Vec P2p, Mat mp) {
		P0 = P0p;
		P1 = P1p;
		P2 = P2p;
		material = mp;
		Plane plane = new Plane(P0, (P1-P0).cross(P2-P0), material);
	}
	Intersection intersect(Ray ray, T tmin, T tmax) {
		Intersection isect = plane.intersect(ray, tmin, tmax);
		// barycentric calculation for triangle intersection
		if(isect != null) {
			Vec v0 = P2-P0;
			Vec v1 = P1-P0;
			Vec v2 = ray.origin-P0;
			T dot00 = v0.dot(v0);
			T dot01 = v0.dot(v1);
			T dot02 = v0.dot(v2);
			T dot11 = v1.dot(v1);
			T dot12 = v1.dot(v2);
			T invDenom = dot00 * dot11 - dot01 * dot01;
			T u = (dot11 * dot02 - dot01 * dot12) / invDenom;
			T v = (dot00 * dot12 - dot01 * dot02) / invDenom;
			if(u >= 0 && v >= 0 && u + v < 1) {
				return isect;
			}
		}
		return null;
	}
}

class BVHNode {
public: 
	Shape shape;
	BVHNode left;
	BVHNode right;
	BB BBox;
	bool isShape;
	BVHNode(Shape sp, BVHNode lp, BVHNode rp, BB bbp, bool ip) {
		shape = sp;
		BVHNode lp;
		BVHNode rp;
		BBox = bbp;
		isShape = ip;
	}
}


class BVH {
public:
	std::vector<Shape> unbounded;
	std::vector<BVHNode> bvhTree;
	BVH(std::vector<Shape> shapes) {
		for (int i = 0; i < shapes.size(); i++) {
			BB BBox = shapes[i].boundingBox();
			if (BBox != NULL) {
				bvhTree.push_back(new BVHNode(shapes[i], NULL, NULL, BBox, true));
			} else {
				unbounded.push_back(shapes[i]);
			}
		}
		BVHNode popTree(shapeArr) {
			if (shapeArr.size() == 1) {
				return shapeArr[0];
			}
			BB BBox = new BoundingBox(shapeArr[0].min, shapeArr[0].max);
			for (let i = 0; i < shapeArr.size(); ++i) {
				BBox.union(shapeArr[i].bbox);
			}
			Vec3 boxSize = BBox.max - BBox.min;
			Vec3 temp = new Vec3();
			// Split along longest dimension of outer bounding box
			struct sortFunc {
				bool operator() (BVHNode p1, BVHNode p2) {
					Vec3 center1 = boxSize * (1/2) + BBox.min;
					return temp1 >= temp2;
				}
			}
			std::sort(shapeArr.begin(), shapeArr.end(), sortFunc);

			let boxSize = BBox.max.clone().sub(BBox.min);
			let temp = new THREE.Vector3();
			// Split along longest dimension of outer bounding box
			let boxCenter = function(box) { 
				let center = new THREE.Vector3();
				box.getCenter(center);
				return center;
			};
			let sortLambda = null;
			if (boxSize.x > boxSize.y && boxSize.x > boxSize.z) {
				sortLambda = function(box) { return boxCenter(box).x; };
			} else if (boxSize.y > boxSize.x && boxSize.y > boxSize.z) {
				sortLambda = function(box) { return boxCenter(box).y; };
			} else {
				sortLambda = function(box) { return boxCenter(box).z; };
			}
			shapeArr = shapeArr.sort(function(b1, b2) { return sortLambda(b1.bbox) - sortLambda(b2.bbox); });
			return new BVHNode(NULL, 
													BBox, 
													popTree(shapeArr.slice(0, floor(shapeArr.size() / 2))), 
													popTree(shapeArr.slice(floor(shapeArr.size() / 2), shapeArr.length)), 
													false);
		}
		if (bvhTree.size() == 0) {
			bvhTree = BVHNode(NULL, new BoundingBox(), false);
		}
		bvhTree = popTree(bvhTree);
	}
}
