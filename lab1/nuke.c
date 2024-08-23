/* Viwesh Gupta
File started 8/21/24 finished on 8/22/24
Nuclear blast lab- an introduction to the c language
Reading in coordinates and name to calculate the dosage of radiation that a person recieved from a nuke
*/

#include <stdio.h>
#include <math.h>

// given struct
struct Point {
    int x;
    int y;
};

// using the math library I set the distance as the distance formula given in the lab write-up
static double distance(const struct Point *p1, const struct Point *p2){
    return sqrt(pow((p1->x - p2->x),2) + pow((p1->y - p2->y),2));
};

int main(int argc, char **argv){
    // error checking for extra or not enough arguments
    if(argc != 5){
        printf("Usage: ./nuke <blast strength> <attenuation> <blast x> <blast y>\n");
        return 1;
    }

    // reading in arguments and setting their values
    float blaststrength;
    double attenuation;
    int blastx;
    int blasty;
    // used an if/else statement to error check the data type
    // if it worked correctly then it just sets the value
    if(sscanf(argv[1], "%f", &blaststrength)){}
    else{
        printf("Error: unable to read blast strength.\n");
        return 1;
    }
    if(sscanf(argv[2], "%lf", &attenuation)){}
    else{
        printf("Error: unable to read blast attenuation.\n");
        return 1;
    }
    if(sscanf(argv[3], "%d", &blastx)){}
    else{
        printf("Error: unable to read blast x coordinate.\n");
        return 1;
    }
    if(sscanf(argv[4], "%d", &blasty)){}
    else{
        printf("Error: unable to read blast y coordinate.\n");
        return 1;
    }

    // set the point based off the given arguments for later use
    struct Point p1;
    p1.x = blastx;
    p1.y = blasty;

    // initiating values for data that is being read in
    int ix;
    int iy;
    char iname[64];

    // use a loop to read in data as X Y Name format the input file until EOF is reached, so it's not an infinite loop
    while(scanf("%d %d %s", &ix, &iy, iname) != EOF){
        // error checking to make sure that the correct data was passed
        if(!scanf("%d %d %s", &ix, &iy, iname)){
            return 1;
        }
        // set the second point given from the file 
        struct Point p2;
        p2.x = ix;
        p2.y = iy;

        //using the distance function defined earlier calculate the distance
        double d = distance(&p1,&p2);

        // using the distance calculated above, the radiation exposed is calculated by setting that distance as the power for the attenuation 
        //which is then multiplied back to the blast strength to calulate exposure
        double sv = blaststrength * pow(attenuation, d);
        
        printf("%-8s: %8.3lf Sv\n", iname, sv);
    }

}
