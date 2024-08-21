/* Viwesh Gupta
File started 8/21/24 finished on 
Nuclear blast lab- an introduction to the c language
Reading in coordinates and name to calculate the dosage of radiation that person recieved*/

#include <stdio.h>
#include <math.h>

struct Point {
    int x;
    int y;
};

static double distance(const struct Point *p1, const struct Point *p2);

int main(int argc, char **argv){
    // error checking for extra arguments
    if(argc != 5){
        printf("Usage: ./nuke <blast strength> <attenuation> <blast x> <blast y>\n");
        return 1;
    }

    // reading in the arguments and setting the values
    float blaststrength;
    double attenuation;
    int blastx;
    int blasty;
    sscanf(argv[1], "%f", &blaststrength);
    sscanf(argv[2], "%lf", &attenuation);
    sscanf(argv[3], "%d", &blastx);
    sscanf(argv[4], "%d", &blasty);

    int ix;
    int iy;
    char iname[64];

    scanf("%d", &ix);
    scanf("%d", &iy);
    scanf("%s", iname);

    printf("%d\n", ix);
    printf("%d\n", iy);
    printf("%s\n", iname);


}