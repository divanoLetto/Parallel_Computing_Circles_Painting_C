#ifndef CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_LAYERS2_H
#define CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_LAYERS2_H

#include <list>
#include <Graphics/Image.hpp>
#include <iostream>
#include <thread>
#include "MyCircle.h"
#include "pixel_seq.h"

void draw_circles_by_pixels_in_layers2(const std::vector<MyCircle>&circle_list,int *pixels, int* sovrapp_matrices, int start_t, int end_t, int width, int height, int num_thread){
    int num_channels = 4;
    float alpha = (float) 128 / (255);
    for(int k = start_t; k<end_t; k++) {
        MyCircle c = circle_list[k];
        int r = c.getRadius();
        int cx = c.getPosx();
        int cy = c.getPosy();
        for (int i = cx; i <cx + 2*r; i++) {
            for (int j = cy; j < cy + 2*r; j++) {
                if (distance(i,j,cx+r,cy+r)< r and i<width and j<height and i>=0 and j>=0){
                    long id = (i + j * width) * num_channels;
                    sovrapp_matrices[id]++;
                    sovrapp_matrices[id+1]++;
                    sovrapp_matrices[id+2]++;

                    if(pixels[id]==-1) {
                        pixels[id] = (sf::Uint8) ((float) c.getColorR() * alpha );
                        pixels[id + 1] = (sf::Uint8) ((float) c.getColorG() * alpha);
                        pixels[id + 2] = (sf::Uint8) ((float) c.getColorB() * alpha);
                    }
                    else {
                        pixels[id] = (sf::Uint8) ((float) c.getColorR() * alpha + (float) pixels[id] * (1 - alpha));
                        pixels[id + 1] = (sf::Uint8) ((float) c.getColorG() * alpha + (float) pixels[id + 1] * (1 - alpha));
                        pixels[id + 2] = (sf::Uint8) ((float) c.getColorB() * alpha + (float) pixels[id + 2] * (1 - alpha));
                    }
                }
            }
        }
    }
}

void paint_pixels_parallel_in_layers2(const std::vector<MyCircle>& circle_vector, int width, int height, int num_threads){
    sf::Image image;
    int num_channels=4;
    float alpha = (float) 128 / 255;
    int *layers_pixels[num_threads];
    int *layers_sovrapp_matrices[num_threads];
    auto *image_pixels  = new sf::Uint8[width * height * num_channels];
    auto *image_sovrapp_matrix  = new sf::Uint8[width * height * num_channels];
    for (int i = 0; i < num_threads; ++i) {
        layers_pixels[i]=new int[width * height * num_channels];
        layers_sovrapp_matrices[i]=new int[width * height * num_channels];
        for (int j = 0; j < width * height * num_channels; j++) {
            layers_pixels[i][j] = -1;
            layers_sovrapp_matrices[i][j]=0;
            image_sovrapp_matrix[j]=0;
        }
    }
    std::vector<std::thread> threads;
    int start_t=0;
    int end_t;
    int num_circle_per_thread = (int)(circle_vector.size()/num_threads);
    int rest = (int)circle_vector.size()%num_threads;

    for (int k=0; k<num_threads; k++){
        end_t = start_t + num_circle_per_thread ;
        if(k<rest){
            end_t++;
        }
        threads.emplace_back(std::thread([&circle_vector, &layers_pixels,&layers_sovrapp_matrices, k, start_t, end_t, width, height](){ draw_circles_by_pixels_in_layers2(circle_vector, layers_pixels[k], layers_sovrapp_matrices[k], start_t, end_t, width, height, k); }));
        start_t = end_t;
    }
    for (auto &t: threads) {
        t.join();
    }
    for(int i=0; i<num_threads; i++) {
        for (int id = 0; id < width * height * num_channels; id++) {
            image_sovrapp_matrix[id]+=layers_sovrapp_matrices[i][id];
        }
    }
    for(int i=0; i<num_threads; i++){
        for(int id = 0; id< width * height * num_channels; id++){
            if(i==0){
                image_pixels[id]= 255 * pow(1 - alpha, image_sovrapp_matrix[id]);
            }
            if(layers_pixels[i][id] != -1 and layers_pixels[i][id] != 255) {
                image_sovrapp_matrix[id]-=layers_sovrapp_matrices[i][id];
                image_pixels[id] += (sf::Uint8) ((float) layers_pixels[i][id] * pow(1 - alpha, image_sovrapp_matrix[id]));
            }
        }
    }
    image.create(width, height, image_pixels);
    image.saveToFile("pixels_para_in_layers2.png");
    delete [] image_pixels;
    delete [] image_sovrapp_matrix;
    for (int i = 0; i < num_threads; ++i) {
        delete[]layers_pixels[i];
        delete [] layers_sovrapp_matrices[i];
    }
}


#endif //CIRCLE_PAINT_PROJECT_PIXEL_PARALLEL_LAYERS2_H
