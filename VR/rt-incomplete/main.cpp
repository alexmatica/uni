#include <cmath>
#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Line.hpp"
#include "Geometry.hpp"
#include "Sphere.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Material.hpp"

#include "Scene.hpp"

using namespace std;
using namespace rt;

double imageToViewPlane(int n, int imgSize, double viewPlaneSize) {
    double u = (double)n*viewPlaneSize / (double)imgSize;
    u -= viewPlaneSize / 2;
    return u;
}

const Intersection findFirstIntersection(const Line& ray,
    double minDist, double maxDist) {
    Intersection intersection;

    for (int i = 0; i < geometryCount; i++) {
        Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
        if (in.valid()) {
            if (!intersection.valid()) {
                intersection = in;
            }
            else if (in.t() < intersection.t()) {
                intersection = in;
            }
        }
    }

    return intersection;
}

int main() {
    Vector viewPoint(0, 0, 0);
    Vector viewDirection(0, 0, 1);
    Vector viewUp(0, -1, 0);

    double frontPlaneDist = 0;
    double backPlaneDist = 1000;

    double viewPlaneDist = 512;
    double viewPlaneWidth = 1024;
    double viewPlaneHeight = 768;

    int imageWidth = 1024;
    int imageHeight = 768;

    Vector viewParallel = viewUp^viewDirection;

    viewDirection.normalize();
    viewUp.normalize();
    viewParallel.normalize();

    Image image(imageWidth, imageHeight);

    for (int i=0; i < imageWidth; i++)
        for (int j=0; j<imageHeight; j++) {
            Vector x0 = viewPoint;
            Vector x1 = viewPoint + viewDirection * viewPlaneDist +
                        viewUp * imageToViewPlane(j, imageHeight, viewPlaneHeight) +
                        viewParallel * imageToViewPlane(i, imageWidth, viewPlaneWidth);
            Line l(x0, x1, false);
            Intersection in = findFirstIntersection(l, frontPlaneDist, backPlaneDist);
            if (in.valid()){
                Sphere* intS = (Sphere*)in.geometry();
                Vector O = intS->center();
                Vector V = in.vec();
                Vector N = (V-O);
                double norm = N.normalize();

                Vector E = (viewPoint - V);
                norm = E.normalize();
                Color c = intS->material().ambient();

                for (int idx = 0; idx < lightCount; idx++){
                    Vector L = lights[idx]->position();
                    Vector T = (L-V);
                    norm = T.normalize();

                    Vector R = N * (N * T) * 2 - T;
                    norm = R.normalize();

                    if (N * T > 0){
                        c += intS->material().diffuse() * lights[idx]->diffuse() * (N*T);
                    }

                    if (E * R > 0){
                        c += intS->material().specular() * lights[idx]->specular() * pow(E * R, intS->material().shininess());
                    }

                    c *= lights[idx]->intensity();

                }
                image.setPixel(i, j, c);
            }
            else{
                image.setPixel(i, j, Color(0,0,0));
            }
    }

    image.store("scene.png");

    for (int i = 0; i < geometryCount; i++) {
        delete scene[i];
    }

    return 0;
}
