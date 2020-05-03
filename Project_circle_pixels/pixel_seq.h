#include <SFML/Graphics.hpp>
#include <list>
#include "MyCircle.h"
#include <math.h>

#ifndef CIRCLE_PAINT_PROJECT_PIXEL_SEQ_H
#define CIRCLE_PAINT_PROJECT_PIXEL_SEQ_H

double distance(long a1, long a2, int b1, int b2){
    return sqrt(pow(a1-b1,2)+pow(a2-b2,2));
}
void paint_pixels_seq(const std::list<MyCircle>& circle_list,int width,int height){
    sf::Image image;
    int num_channels=4;
    float alpha = (float)128/255;
    auto *pixels  = new sf::Uint8[width * height * num_channels];
    for(int i =0; i<width * height * num_channels; i++ ){
        pixels[i]=255;
    }
    for(const auto& c:circle_list) {
        int r = c.getRadius();
        int cx = c.getPosx();
        int cy = c.getPosy();
        for (int i = cx; i <cx + 2*r; i++) {
            for (int j = cy; j < cy + 2*r; j++) {
                if (distance(i,j,cx+r,cy+r)< r and i<width and j<height and i>=0 and j>=0){
                    long id = (i + j * width) * num_channels;
                    pixels[id] = (sf::Uint8)((float)c.getColorR()*alpha + (float)pixels[id]*(1-alpha));
                    pixels[id+1] = (sf::Uint8)((float)c.getColorG()*alpha + (float)pixels[id+1]*(1-alpha));
                    pixels[id+2] = (sf::Uint8)((float)c.getColorB()*alpha + (float)pixels[id+2]*(1-alpha));
                }
            }
        }
    }
    image.create(width, height, pixels);
    image.saveToFile("pixels_sq.png");
    delete []pixels;
}
#endif //CIRCLE_PAINT_PROJECT_PIXEL_SEQ_H
