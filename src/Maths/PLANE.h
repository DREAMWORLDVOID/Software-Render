//////////////////////////////////////////////////////////////////////////////////////////
//	PLANE.h
//	Class declaration for a plane in 3d space
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	6th August 2002		-	Added "Normalize"
//				7th November 2002	-	Altered constructor layout
//									-	Corrected "lerp"
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef PLANE_H
#define PLANE_H

class PLANE {
public:
    PLANE() : normal(Vec3(0.0f, 0.0f, 0.0f)), intercept(0.0f) {}

    PLANE(Vec3 newNormal, float newIntercept) : normal(newNormal), intercept(newIntercept) {}

    PLANE(const PLANE &rhs) {
        normal = rhs.normal;
        intercept = rhs.intercept;
    }

    ~PLANE() {}

    void SetNormal(const Vec3 &rhs) { normal = rhs; }

    void SetIntercept(float newIntercept) { intercept = newIntercept; }

    void SetFromPoints(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2);

    void CalculateIntercept(const Vec3 &pointOnPlane) { intercept = -normal.DotProduct(pointOnPlane); }

    void Normalize(void);

    Vec3 GetNormal() { return normal; }

    float GetIntercept() { return intercept; }

    //find point of intersection of 3 planes
    bool Intersect3(const PLANE &p2, const PLANE &p3, Vec3 &result);

    float GetDistance(const Vec3 &point) const;

    int ClassifyPoint(const Vec3 &point) const;

    PLANE lerp(const PLANE &p2, float factor);

    //operators
    bool operator==(const PLANE &rhs) const;

    bool operator!=(const PLANE &rhs) const { return !((*this) == rhs); }

    //unary operators
    PLANE operator-(void) const { return PLANE(-normal, -intercept); }

    PLANE operator+(void) const { return (*this); }

    //member variables
    Vec3 normal;    //X.N+intercept=0
    float intercept;
};

//constants for ClassifyPoint()
const int POINT_ON_PLANE = 0;
const int POINT_IN_FRONT_OF_PLANE = 1;
const int POINT_BEHIND_PLANE = 2;

#endif //PLANE_H