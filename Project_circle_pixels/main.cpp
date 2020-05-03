#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <list>
#include <iostream>
#include "MyCircle.h"
#include "Utils.h"
#include "pixel_seq.h"
#include "pixel_parallel.h"
#include "pixel_parallel_layers2.h"

int main() {
    using namespace std;
    std::cout<<"Start main:"<<std::endl;

    int width = 1000;
    int height = 1000;
    int max_prfontidy = 1000;
    int num_exp = 10;
    int num_circle = 10000;
    int num_thread = 8;
    vector<int> choices = {10000};

    int min_radius = 20;
    int max_radius = 60;

    for(auto choice: choices) {
        vector<int> time_pixels_sq = {};
        vector<int> time_pixels_para_in_zone= {};
        vector<int> time_pixels_para_in_layers={};
        num_circle = choice;
        cout<<"Start exp num circles= "<<num_circle<< " and num threads= "<<num_thread<< " and width x height= "<<width<<"x"<<height<<endl;
        generate_random_circle_to_file(num_circle, min_radius, max_radius, width, height, max_prfontidy, num_exp, num_thread);

        for (int i = 0; i < num_exp; i++) {
            list<MyCircle> circle_list = get_circle_list_from_file(num_circle, i, num_thread, width, height);
            circle_list.sort();
            vector<MyCircle> circle_vector{begin(circle_list), end(circle_list)};

            cout << "Start pixel sequential version: " << endl;
            chrono::steady_clock::time_point begin_pixels_sq = chrono::steady_clock::now();
            paint_pixels_seq(circle_list, width, height);
            chrono::steady_clock::time_point end_pixels_sq = chrono::steady_clock::now();

            //cout << "Start pixel parallel in zone version: " << endl;
            chrono::steady_clock::time_point begin_pixels_para = chrono::steady_clock::now();
            paint_pixels_parallel(circle_list, width, height, num_thread);
            chrono::steady_clock::time_point end_pixels_para = chrono::steady_clock::now();

            //cout << "Start pixel parallel version in layers: " << endl;
            chrono::steady_clock::time_point begin_pixels_para_in_layers2 = chrono::steady_clock::now();
            paint_pixels_parallel_in_layers2(circle_vector, width, height, num_thread);
            chrono::steady_clock::time_point end_pixels_para_in_layers2 = chrono::steady_clock::now();

            time_pixels_sq.push_back(chrono::duration_cast<chrono::microseconds>(end_pixels_sq - begin_pixels_sq).count());
            time_pixels_para_in_zone.push_back(chrono::duration_cast<chrono::microseconds>(end_pixels_para - begin_pixels_para).count());
            time_pixels_para_in_layers.push_back(chrono::duration_cast<chrono::microseconds>(end_pixels_para_in_layers2 - begin_pixels_para_in_layers2).count());
        }
        int media_pixels_sq = 0,  media_pixels_para = 0, media_pixels_para_layers2=0;
        for (int j = 0; j < num_exp; j++) {
            media_pixels_sq += time_pixels_sq[j];
            media_pixels_para +=time_pixels_para_in_zone[j];
            media_pixels_para_layers2 +=time_pixels_para_in_layers[j];
        }
        media_pixels_sq = media_pixels_sq /num_exp;
        media_pixels_para = media_pixels_para /num_exp;
        media_pixels_para_layers2 = media_pixels_para_layers2 /num_exp;
        float speedup_pixels = (float)media_pixels_sq/(float)media_pixels_para;
        float speedup_pixels_layers2 = (float)media_pixels_sq/(float)media_pixels_para_layers2;

        cout << "Time pixels sequential version  = " << media_pixels_sq << "[µs]" << endl;
        cout << "Time pixels parallel version    = " << media_pixels_para << "[µs]" << endl;
        cout << "Time pixels parallel layers2    = " << media_pixels_para_layers2 << "[µs]" << endl;

        cout << "The pixels zone speedup is      = " << speedup_pixels <<endl;
        cout << "The pixels layers speedup is    = " << speedup_pixels_layers2 <<endl;
    }
    return 0;
}
