#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lodepng.h"

unsigned char* load_png(const char* filename, unsigned int* width, unsigned int* height)
{
    unsigned char* image = NULL;
    int error = lodepng_decode32_file(&image, width, height, filename);
    if(error != 0) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
    return (image);
}

void write_png(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
    unsigned char* png;
    long long unsigned int pngsize;
    int error = lodepng_encode32(&png, &pngsize, image, width, height);
    if(error == 0) {
        lodepng_save_file(png, pngsize, filename);
    } else {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
    free(png);
}


void contrast(unsigned char *col, int bw_size){
    for(int i=0; i < bw_size; i++){
        if(col[i] < 55)
            col[i] = 0;
        if(col[i] > 195)
            col[i] = 255;
    }
    return;
}

int check(unsigned char *pic, int size, int i, int neighbour){
    if (i >= 0 && i < size && neighbour >= 0 && neighbour < size){
        if (pic[i] - pic[neighbour] > 30){
            return 1;
        }
    }
    return 0;
}



int set_contrast(int bw_size, unsigned char* bw_pic, unsigned char* picture, int width) {
    unsigned char value;
    for (int i = 0; i < bw_size; i++){
        value = (picture[i * 4] + picture[i * 4 + 1] + picture[i * 4 + 2]) / 3;
        bw_pic[i] = value;
    }
    for (int i = 0; i < bw_size; i++) {
        if (check(bw_pic, bw_size, i, i - 1)||
            check(bw_pic, bw_size, i, i + 1)||
            check(bw_pic, bw_size, i, i - width)||
            check(bw_pic, bw_size, i , i + width)){
            bw_pic[i] = 255;
        }
    }
    for (int i = 0; i < bw_size; i++) {
        if (bw_pic[i] < 255) {
            bw_pic[i] = 0;
        }
    }
    return 1;
}


int fill(unsigned char* pic, unsigned char* mat, int width){
    //координаты прямоугольников - см карт. ЕЕ в качестве попытки улучшения видимости заапскейлил
    int sectors[9][2][2] = {
            {{1149, 435},{1423, 39}},
            {{2153, 27},{2887, 1151}},
            {{2025, 1251},{3733, 1189}},
            {{2199, 1273},{4521, 1393}},
            {{1969, 1373},{2197, 1393}},
            {{2033, 1395},{4535, 1607}},
            {{2127, 1611},{4581, 2233}},
            {{2261, 2239},{4681, 2413}},
            {{2393, 2415},{4705, 2587}}
    };
    for (int i = 0; i < 9; i++){
        int start;
        if (sectors[i][0][1] < sectors[i][1][1]) {
            start = sectors[i][0][1];
        }
        else{
            start = sectors[i][1][1];
        }
        int end;
        if (sectors[i][0][1] > sectors[i][1][1]) {
            end = sectors[i][0][1];
        }
        else{
            end = sectors[i][1][1];
        }
        for (int j = sectors[i][0][0]; j < sectors[i][1][0]; j++){
            for (int k = start; k < end; k++){
                if (pic[k * width + j]) {
                    mat[k * width + j] = 255;
                }
            }
        }
    }
    return 1;
}


int dfs(unsigned char* bw_pic, unsigned char* visited, int current, int bw_size, unsigned char* mat, int width) {
    if (current < 0 || current >= bw_size){
        return 0;
    }
    if (visited[current] || (mat[current] == 0)){
        return 0;
    }
    visited[current] = 1;
    dfs(bw_pic, visited, current + 1, bw_size, mat, width);
    dfs(bw_pic, visited, current - 1, bw_size, mat, width);
    dfs(bw_pic, visited, current + width, bw_size, mat, width);
    dfs(bw_pic, visited, current - width, bw_size, mat, width);
    dfs(bw_pic, visited, current + 1 + width, bw_size, mat, width);
    dfs(bw_pic, visited, current - 1 + width, bw_size, mat, width);
    dfs(bw_pic, visited, current + 1 - width, bw_size, mat, width);
    dfs(bw_pic, visited, current - 1 - width, bw_size, mat, width);
    return 0;
}


int main(void)
{
    const char* filename = "photo_2026-03-05_12-50-10_upscayl_4x_realesrgan-x4plus.png";
    unsigned int width, height;
    int bw_size;
    unsigned char* picture = load_png(filename, &width, &height);
    if (picture == NULL)
    {
        printf("Problem reading picture from the file %s. Error.\n", filename);
        return -1;
    }
    int counter = 0;
    bw_size = width * height;
    unsigned char* bw_pic = (unsigned char*)calloc(bw_size, sizeof(unsigned char));
    unsigned char* visited = (unsigned char*)calloc(bw_size, sizeof(unsigned char));
    unsigned char* territory = (unsigned char*)calloc(bw_size,sizeof(unsigned char));
    set_contrast(bw_size, bw_pic, picture, width);
    contrast(bw_pic, bw_size);
    fill(bw_pic, territory, width);
    for (int i = 0; i < bw_size; i++){
        if (!visited[i] && territory[i] != 0){
            dfs(territory, visited, i, bw_size, territory, width);
            counter++;
        }
    }
    printf("%d", counter);
    free(bw_pic);
    free(visited);
    free(territory);
    free(picture);
    return 0;
}
