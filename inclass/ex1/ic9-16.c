#include <stdio.h>

struct MyStruct
{
    int i, j;
    float k;
};


// write
// int main(int argc, char* argv[]){
//     FILE *fout;
//     struct MyStruct ms = {111,  -222, 33.5F};

//     fout = fopen("myfile.bin", "wb");
//     if(!fout){
//         perror("myfile.bin");
//         return -1;
//     }
//     size_t ret = fwrite(&ms, 1,  sizeof(ms), fout);
//     if(ret != sizeof(ms)) {
//         perror("fwrite");
//         return -1;
//     }

//     fclose(fout);
// }

// read
int main(int argc, char* argv[]){
    FILE *fin;
    struct MyStruct ms;
    char data;

    fin = fopen("myfile.bin", "rb");
    if(fin == NULL){
        perror("myfile.bin");
        return -1;
    }
    size_t ret = fread(&ms, 1,  sizeof(ms), fin);
    if(ret != sizeof(ms)) {
        perror("fread");
        return -1;
    }

    // fseek(fin, 0, SEEK_SET);
    fseek(fin,-4,SEEK_CUR);

    printf("We are %lu bytes into the file.\n", ftell(fin));

    fread(&data,1, 1, fin);

    printf("%x", data);

    fclose(fin);

    printf("%d %d %f\n", ms.i, ms.j, ms.k);
}
