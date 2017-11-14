#include "Sphere.hpp"

using namespace rt;

Intersection Sphere::getIntersection(const Line& line, double minDist, double maxDist) {
    Intersection in;
    /*
        http://www.ambrsoft.com/TrigoCalc/Sphere/SpherLineIntersection_.htm
    */
    double dX_x = line.dx().x();
    double dX_y = line.dx().y();
    double dX_z = line.dx().z();

    double x0_x = line.x0().x();
    double x0_y = line.x0().y();
    double x0_z = line.x0().z();

    double xC_x = this->center().x();
    double xC_y = this->center().y();
    double xC_z = this->center().z();

    double a = dX_x * dX_x + dX_y * dX_y + dX_z * dX_z;
    double b = 2 *
        (dX_x * (x0_x - xC_x) +
         dX_y * (x0_y - xC_y) +
         dX_z * (x0_z - xC_z));
    double c = xC_x * xC_x + xC_y * xC_y + xC_z * xC_z +
                x0_x * x0_x + x0_y * x0_y + x0_z * x0_z -
                2 * (x0_x * xC_x + x0_y * xC_y + x0_z * xC_z) -
                this->radius() * this->radius();

    double delta = b * b - 4 * a * c;

    if (delta > 0){
        double t1 = (-b + sqrt(delta)) / (2*a);
        double t2 = (-b - sqrt(delta)) / (2*a);

        if (t1 < t2){
            if (t1 > minDist && t1 < maxDist)
                in = Intersection(true, this, &line, t1);
        }
        else{
            if (t2 > minDist && t2 < maxDist)
                in = Intersection(true, this, &line, t2);
        }
    }
    else if (delta == 0){
        double t = -b / (2*a);
        if (t > minDist && t < maxDist)
            in = Intersection(true, this, &line, t);
    }
    return in;
}


const Vector Sphere::normal(const Vector& vec) const {
    Vector n = vec - _center;
    n.normalize();
    return n;
}
