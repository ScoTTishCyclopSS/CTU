#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
 
#define FILE_ERR "Can't open the file\n"
#define PPM_STRUCT_ALLOC_ERR "Can't allocate PPMImage struct\n"
#define IMG_INFO_CURR "Read img info error\n"
#define IMG_DATA_ALLOC_ERR "Can't allocate PPMImage data\n"
#define PXL_LOAD_ERR "Can't load pixels\n"
 
#define LOAD_PPM_ERR 1
#define RED_ROUND 0.2126
#define GREEN_ROUND 0.7152
#define BLUE_ROUND 0.0722
 
typedef struct {
    unsigned char r, g, b;
} PPMPxl;
 
typedef struct {
    int x, y, constant;
    PPMPxl *data;
} PPMImg;
 
static PPMImg *loadPPM (const char *file);
static int calculateGrayscale  (int r, int g, int b);
static PPMImg *constructPPMfrom (int x, int y, int constant);
static void setPPM (PPMImg *img);
static void saveHist(int *histogram);
 
int main(int argc, char* argv[]) {
    int histogram[] = {0, 0, 0, 0, 0, 0};
    int grayscale = 0, red, green, blue;
 
    PPMImg *img = loadPPM(argv[1]); 
    if (img == NULL) {
        free(img);
        return LOAD_PPM_ERR;
    }  
         
    PPMImg *new_img = constructPPMfrom(img->x, img->y, img->constant); // x, y, constant are loaded to the new image
 
    for (int i = 0; i < img->x; i++) {// the first row
        new_img->data[i] = img->data[i]; // 1 pixel -> 1 pixel
        grayscale = calculateGrayscale(new_img->data[i].r, new_img->data[i].g, new_img->data[i].b);
        histogram[grayscale]++;
    }
     
    for (int i = img->x; i < (img->x * img->y) - img->x; i += img->x) {// from the second to the pre-last
        new_img->data[i] = img->data[i];
        grayscale = calculateGrayscale(new_img->data[i].r, new_img->data[i].g, new_img->data[i].b);
        histogram[grayscale]++;
         
        for (int j = i + 1; j < i + img->x - 1; j++) {// i < ???.....
            // creating convolution mask
            red = 5 * img->data[j].r - img->data[j - 1].r - img->data[j + 1].r - img->data[j - img->x].r - img->data[j + img->x].r;
            green = 5 * img->data[j].g - img->data[j - 1].g - img->data[j + 1].g - img->data[j - img->x].g - img->data[j + img->x].g; 
            blue = 5 * img->data[j].b - img->data[j - 1].b - img->data[j + 1].b - img->data[j - img->x].b - img->data[j + img->x].b;
 
            // setting new RGB values
            new_img->data[j].r = (red > 0) ? ((red < 256) ? (red) : (255)) : (0);
            new_img->data[j].g = (green > 0) ? ((green < 256) ? (green) : (255)) : (0);
            new_img->data[j].b = (blue > 0) ? ((blue < 256) ? (blue) : (255)) : (0);
 
            grayscale = calculateGrayscale(new_img->data[j].r, new_img->data[j].g, new_img->data[j].b);
            histogram[grayscale]++;
        }
        new_img->data[i + img->x - 1] = img->data[i + img->x - 1];
        grayscale = calculateGrayscale(new_img->data[i + img->x - 1].r, new_img->data[i + img->x - 1].g, new_img->data[i + img->x - 1].b);
        histogram[grayscale]++;
    }
     
    for (int i = (img->x * img->y) - img->x; i < (img->x * img->y); i++) { // the last row
        new_img->data[i] = img->data[i]; // 1 pixel -> 1 pixel
        grayscale = calculateGrayscale(new_img->data[i].r, new_img->data[i].g, new_img->data[i].b);
        histogram[grayscale]++;
    }
 
    // close the old image
    free(img->data);
    free(img);
 
    // save the new img
    setPPM(new_img);
    free(new_img->data);
    free(new_img);
 
    // save the histogram
    saveHist(histogram);
 
    return 0;
}
 
static void saveHist(int *histogram) {
    FILE *f = fopen("output.txt", "wb");
    fprintf(f, "%d %d %d %d %d", 
            histogram[0], 
            histogram[1], 
            histogram[2], 
            histogram[3], 
            histogram[4] + histogram[5]
        );
    fclose(f);
}
 
static void setPPM (PPMImg *img) {
    // write output.ppm
    FILE *f = fopen("output.ppm", "wb");
    fprintf(f, "P6\n%d\n%d\n%d\n", img->x, img->y, img->constant);
    fwrite(img->data, img->x * 3, img->y, f);
    fclose(f);
}
 
static PPMImg *constructPPMfrom (int x, int y, int constant) {
    PPMImg *ppm_img = malloc(sizeof(PPMImg));
    if (!ppm_img) {
        fprintf(stderr, PPM_STRUCT_ALLOC_ERR);
        return NULL;
    }
     
    ppm_img->x = x;
    ppm_img->y = y;
    ppm_img->constant = constant;
 
    ppm_img->data = malloc(sizeof(PPMPxl) * (x * y));
    if (!ppm_img->data) {
        fprintf(stderr, IMG_DATA_ALLOC_ERR);
        return NULL;
    }
    return ppm_img;
}
 
static PPMImg *loadPPM (const char *file) {
    FILE *f = fopen(file, "rb");
    if (!f) {
        fprintf(stderr, FILE_ERR);
        return NULL;
    }
         
    //allocate memory for img
    PPMImg *ppm_img = malloc(sizeof(PPMImg));
    if (!ppm_img) {
        fprintf(stderr, PPM_STRUCT_ALLOC_ERR);
        return NULL;
    }
 
    //you know I should have checked the file format, 
    //but I trust CW, so It might always be P6   
    int r = fscanf(f, "P6 %d %d %d\n", &ppm_img->x, &ppm_img->y, &ppm_img->constant);
    if (r != 3) {
        fprintf(stderr, IMG_INFO_CURR);
        return NULL;
    }
     
    //load the data directly
    int size = ppm_img->x * ppm_img->y;
    ppm_img->data = malloc(sizeof(PPMPxl) * size);
    if (!ppm_img->data) {
        free(ppm_img->data);
        fprintf(stderr, IMG_DATA_ALLOC_ERR);
        return NULL;
    }
 
    //load pixels
    if(!fread(ppm_img->data, ppm_img->x*3, ppm_img->y, f)) {
        fprintf(stderr, PXL_LOAD_ERR);
        return NULL;
    }
     
    fclose(f);
    return ppm_img;
}
 
static int calculateGrayscale  (int r, int g, int b) {
    return (RED_ROUND * r + GREEN_ROUND * g + BLUE_ROUND * b + 0.5) / 51;
}
