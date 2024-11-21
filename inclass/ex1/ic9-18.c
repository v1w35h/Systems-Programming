#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>  // meta data of a file

static size_t mystrcspn(const char *line, const  char *rejects){
    int i, j;
    for(i=0;line[i] != '\0'; i+=1) {
        for(j = 0; rejects[j] != '\0'; j+=1){
            if(line[i]== rejects[j]){
                goto out;
            }
        }
    }
    out:
    return i;
}


// int main(void) {
//     FILE *fl;
//     char line[256];
//     int value;

//     fl = fopen("text.txt", "r");
//     // fscanf(fl, "%255s %d", line, &value);
//     while(fgets(line,256,fl)){
//         // printf("%s\n",line);
//         // fputs(line, stdout);
//         line[mystrcspn(line,"\r\n")] = '\0';
//         printf("%s: %lu\n", line, mystrcspn(line, "\n"));
//     }
//     fclose(fl);

//     // printf("String = '%s'\n", line);

//     // printf("Integer = %d\n",value);

// }

int main(void) {
    char  wd[256];
    struct stat st;


    getcwd(wd, 255);
    // chdir("/");

    // mkdir("mydir", 0777);
    // if(-1==rmdir("mydir")){
    //     perror("mydir");
    // }


    // strcat(wd, "/text.txt");
    // printf("'%s'\n", wd);
}