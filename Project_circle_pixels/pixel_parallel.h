#ifndef CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_H
#define CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_H

#include <list>
#include <Graphics/Image.hpp>
#include <iostream>
#include <thread>
#include "MyCircle.h"
#include "pixel_seq.h"

void draw_circles_by_pixels(const std::list<MyCircle>&circle_list,sf::Uint8 *pixels, int start_x, int end_x, int start_y, int end_y,int width , int thread_id){
    int num_channels = 4;
    float alpha = (float) 128 / 255;
    for(const auto& c:circle_list) {
        int r = c.getRadius();
        int cx = c.getPosx();
        int cy = c.getPosy();
        if(!(cx>end_x or cx + 2*r < start_x or cy>end_y or cy + 2*r < start_y)) {
            for (int i = cx; i < cx + 2 * r; i++) {
                for (int j = cy; j < cy + 2 * r; j++) {
                    if (i < end_x and j < end_y and i >= start_x and j >= start_y) {
                        if(distance(i, j, cx + r, cy + r) < (double) r ) {
                            long id = (i + j * width) * num_channels;
                            pixels[id] = ((float) c.getColorR() * alpha + (float) pixels[id] * (1 - alpha));
                            pixels[id + 1] = ((float) c.getColorG() * alpha + (float) pixels[id + 1] * (1 - alpha));
                            pixels[id + 2] = ((float) c.getColorB() * alpha + (float) pixels[id + 2] * (1 - alpha));
                        }
                    }
                }
            }
        }
    }
}

void paint_pixels_parallel(const std::list<MyCircle>& circle_list, int width, int height, int num_threads){
    sf::Image image;
    int num_channels=4;
    sf::Uint8 *pixels  = new sf::Uint8[width * height * num_channels];
    for(int i = 0; i<width * height * num_channels; i++ ){
        pixels[i]=255;
    }

    int best_factors1=0;
    int best_factors2=0;
    std::list<int> factors={};
    int min_distance=INFINITY;
    if(num_threads==1){
        best_factors1=1;
        best_factors2=1;
    }
    else {
        for (int i = 1; i < num_threads; i++) {
            if (num_threads % i == 0) {
                int distance = abs(i - num_threads / i);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_factors1 = i;
                    best_factors2 = num_threads / i;
                }
            }
        }
    }
    std::vector<std::thread> threads;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    for(int i=0; i<num_threads; i++){
        start_x = (i%best_factors1)*width/best_factors1;
        start_y = (int)floor(i/best_factors1) * height / best_factors2;
        end_x = start_x + width/best_factors1;
        end_y = start_y + height/best_factors2;
        threads.emplace_back(std::thread([&circle_list, &pixels, start_x, end_x, start_y, end_y, width, i](){ draw_circles_by_pixels(circle_list, pixels, start_x, end_x,start_y, end_y, width, i); }));
    }
    for (auto &t: threads) {
        t.join();
    }
    image.create(width, height, pixels);
    image.saveToFile("pixels_para.png");
    delete []pixels;
}

#endif //CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_H
