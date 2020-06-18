
#include "utils.h"
#include "shapes.h"

#define PI 3.14159265359
#define INF 1e8

/* Trace ray in the scene and return spolor of ray. 'depth' is the current recursion depth.
 * If intersection material has non-null kr or kt, perform recursive ray tracing. */
Vec3 raytracing(Ray ray, T depth) {
	let color = new Vec(0, 0, 0);
	let iPoint = rayIntersectScene(ray, BVHTree);
	if (iPoint != null) {
		Ray backRay = new Ray(iPoint.position, -ray.direction);
		if ((iPoint.material.kr == NULL && iPoint.material.kt == NULL) || maxDepth <= depth) {
			color.add(shading(backRay, iPoint));
		} else {
			if (interPoint.material.kr != NULL) {
				color.add(raytracing(new Ray(interPoint.position, reflect(revRay.direction, interPoint.normal)), depth+1) * interPoint.material.kr);
			}
			if (interPoint.material.kt != NULL) {
				Ray refRay = new Ray(interPoint.position, refract(ray.direction, interPoint.normal, interPoint.material.ior));
				if (refRay.direction != NULL) {
					color.add(raytracing(refRay, depth+1) * interPoint.material.kt);
				}
			}
		}
	} else {
		return backgroundColor;
	}
// ---YOUR CODE ENDS HERE---
	return color;
}

/* Compute and return shading color given a ray and the intersection point structure. */
Vec3 shading(Ray ray, Intersection isect) {
	Vec3 color = new Vec(0,0,0);
	if (isect.material.ka != null) {
		color.add(ambientLight * isect.material.ka);
	}
	for (int i = 0; i < lights.length; i++) {
		// WIP: COMPLETE LIGHT CLASS
		let light = lights[i].getLight(isect.position);
		Intersection shadowPoint = rayIntersectScene(new Ray(isect.position, lightInst.direction), BVHTree);
		if (shadowPoint == null || (shadowPoint.t*shadowPoint.t) >= (light.position - isect.position).len2()) {
			if (isect.material.kd != null) {
				T orth = isect.normal.dot(light.direction);
				orth = (orth > 0) ? orth : 0;
				color.add(light.intensity * orth * isect.material.kd);
			}
			if (isect.material.ks != NULL) {
				T reflectOrth = reflect(light.direction, isect.normal).dot(ray.direction);
				reflectOrth = (reflectOrth > 0) ? reflectOrth : 0;
				color.add(light.intensity * pow(reflectOrth, isect.material.p) * isect.material.ks);
			}
		}
	}
	return color;
}

/* Compute intersection of ray with scene shapes.
 * Return intersection structure (null if no intersection). */
Intersection rayIntersectScene(Ray ray, BVH tree) {
	T tmax = INF;
	Intersection isect;
	void minIPoint(BVH node) {
		if (node.isShape) {
			Intersection hit = node.shape.intersect(ray, 0.0001, tmax);
			if (hit != NULL) {
				tmax = hit.t;
				if(isect == NULL) isect = hit; // if this is the first time intersection is found
				else isect.set(hit); // update intersection point
			}
		} else if ((new Ray(ray.o, ray.d)).intersectBox(node.bbox) != NULL) {
			minIPoint(node.left);
			minIPoint(node.right);
		}
	}
	for(int i = 0; i < tree.unbounded.length; i++) {
		Intersection hit = tree.unbounded[i].intersect(ray, 0.0001, tmax);
		if(hit != NULL) {
			tmax = hit.t;
			if(isect == NULL) { isect = hit; } // if this is the first time intersection is found
			else { isect.set(hit); } // update intersection point
		}
	}
	minIPoint(tree.bvhTree);
	return isect;
}


/* Compute reflected vector, by mirroring l around n. */
function reflect(l, n) {
	// r = 2(n.l)*n-l
	Vec3 r = n * ( 2 * n.dot(l));
	r.add(-l);
	return r;
}

/* Compute refracted vector, given l, n and index_of_refraction. */
function refract(l, n, ior) {
	T mu = (n.dot(l) < 0) ? 1/ior : ior;
	T cosI = l.dot(n);
	T sinI2 = 1 - cosI*cosI;
	if(mu*mu*sinI2 > 1) { return null; }
	T sinR = mu*sqrt(sinI2);
	T cosR = sqrt(1-sinR*sinR);
	Vec r = n;
	if(cosI > 0) {
		r = r*(-mu*cosI+cosR) + (l*mu);
	} else {
		r = r*(-mu*cosI-cosR) + (l*mu);
	}
	r.normalize();
	return r;
}
