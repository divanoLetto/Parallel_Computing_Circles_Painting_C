#ifndef CIRCLE_PAINT_PROJECT_UTILS_H
#define CIRCLE_PAINT_PROJECT_UTILS_H

#include <random>
#include "MyCircle.h"
#include <list>
#include <cstring>

void generate_random_circle_to_file(int num_circle, int min_radius, int max_radius, int width, int height, int max_deep_z, int num_exp, int num_thread){

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist_radius(min_radius, max_radius);
    std::uniform_int_distribution<int> dist_posx(-max_radius, width - max_radius);
    std::uniform_int_distribution<int> dist_posy(-max_radius, height - max_radius);
    std::uniform_int_distribution<int> dist_posz(0, max_deep_z);
    std::uniform_int_distribution<int> dist_color_r(0, 255);
    std::uniform_int_distribution<int> dist_color_g(0, 255);
    std::uniform_int_distribution<int> dist_color_b(0, 255);

    FILE *fptr;
    std::string s = "/home/lorenzo/CLionProjects/Project_circle_pixels/test/test_" + std::to_string(width);
    int n = s.length();
    char char_array[n + 1];
    strcpy(char_array, s.c_str());
    if ((fptr = fopen(char_array, "w")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    for (int j =0; j< num_exp; j++) {
        for (int i = 0; i < num_circle; i++) {
            int radius = dist_radius(mt);
            int posx = dist_posx(mt);
            int posy = dist_posy(mt);
            int posz = dist_posz(mt);
            int color_r = dist_color_r(mt);
            int color_g = dist_color_g(mt);
            int color_b = dist_color_b(mt);

            fprintf(fptr, "%d %d %d %d %d %d %d %d\n", i, radius, posx, posy, posz, color_r, color_g, color_b);
        }
    }
    fclose(fptr);
}

std::list<MyCircle> get_circle_list_from_file(int num_circle, int current_exp, int num_threads, int width, int height){
    std::list<MyCircle> list_circles={};
    FILE *fptr;
    std::string s = "/home/lorenzo/CLionProjects/Project_circle_pixels/test/test_" + std::to_string(width);
    int n = s.length();
    char char_array[n + 1];
    strcpy(char_array, s.c_str());
    if ((fptr = fopen(char_array, "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    int id;
    int radius ;
    int posx ;
    int posy ;
    int posz ;
    int color_r ;
    int color_g ;
    int color_b ;
    for( int i = 0; i< current_exp; i++){
        for(int j = 0; j< num_circle; j++){
            fscanf(fptr, "%d %d %d %d %d %d %d %d\n", &id, &radius, &posx, &posy, &posz, &color_r, &color_g, &color_b);
        }
    }
    for(int j = 0; j< num_circle; j++){
        fscanf(fptr, "%d %d %d %d %d %d %d %d\n", &id, &radius, &posx, &posy, &posz, &color_r, &color_g, &color_b);
        MyCircle c(radius, posx, posy, posz, color_r, color_g, color_b);
        list_circles.push_back(c);
    }
    fclose(fptr);
    return list_circles;
}


#endif //CIRCLE_PAINT_PROJECT_UTILS_H
