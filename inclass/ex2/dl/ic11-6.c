// shared libraries & dynamic linker
#include <dlfcn.h>
// compile with -ldl 
#include <stdio.h>
#include <errno.h>

int main(){
    // function pointer, functions have 3 parts
    // register file: holds registers to be loaded x0, x1, x2
    // a0-a7 argument registers
    float (*funky) (int i, int j, int k);
    int ret;
    void *handle;

    handle = dlopen("./libmylib.so", RTLD_LAZY);
        if(!handle){
        // printf("When absolute path is specified~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("%s\n", dlerror());
        return -1;
    }

    funky = dlsym(handle, "myfunctorun");
    if(!funky){
        printf("%s\n", dlerror());
        return -1;
    }

    printf("Funky is at: %p\n", funky);
    ret = funky(10,20,30);

    printf("Funky returned: %d\n", ret);

    dlclose(handle);


    return 0;

}