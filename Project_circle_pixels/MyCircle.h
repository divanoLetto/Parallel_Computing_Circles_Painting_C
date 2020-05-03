#ifndef CIRCLE_PAINT_PROJECT_MYCIRCLE_H
#define CIRCLE_PAINT_PROJECT_MYCIRCLE_H

#include <cmath>

class MyCircle{

private:
    int radius;
    int posx;
    int posy;
    int posz;
    int color_r;
    int color_g;
    int color_b;
public:
    MyCircle(int rad, int px, int py, int pz, int colr, int colg, int colb){
        radius=rad;
        posx=px;
        posy=py;
        posz = pz;
        color_r=colr;
        color_g=colg;
        color_b=colb;
    }
    MyCircle(){
        radius=0;
        posx=0;
        posy=0;
        posz =0;
        color_r=0;
        color_g=0;
        color_b=0;
    }
    MyCircle(MyCircle const &c){
        radius=c.radius;
        posx=c.posx;
        posy=c.posy;
        posz= c.posz;
        color_r=c.color_r;
        color_g=c.color_g;
        color_b=c.color_b;
    }
    int getRadius() const {
        return radius;
    }

    int getPosx() const {
        return posx;
    }

    int getPosy() const {
        return posy;
    }
    int getPosz() const {
        return posz;
    }

    int getColorR() const {
        return color_r;
    }

    int getColorG() const {
        return color_g;
    }

    int getColorB() const {
        return color_b;
    }

    bool operator<(const MyCircle &c) const {
        return posz < c.getPosz();
    }
};


#endif //CIRCLE_PAINT_PROJECT_MYCIRCLE_H
