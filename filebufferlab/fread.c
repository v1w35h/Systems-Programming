#include <stdio.h>
       #include <stdlib.h>

       #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

       int
       main(void)
       {
           FILE           *fp;
           size_t         ret;
           unsigned char  buffer[4];

           fp = fopen("read.txt", "rb");
           if (!fp) {
               perror("fopen");
               return EXIT_FAILURE;
           }


           ret = fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fp);
           if (ret != ARRAY_SIZE(buffer)) {
               fprintf(stderr, "fread() failed: %zu\n", ret);
               exit(EXIT_FAILURE);
           }

           printf("ELF magic: %#04x%02x%02x%02x\n", buffer[0], buffer[1],
                  buffer[2], buffer[3]);

        //    ret = fread(buffer, 10, 10, fp);
        //    printf("%lu\n",ret);
           printf("%d\n", fread(buffer, 1, 100000, fp));
           if (ret != 1) {
               fprintf(stderr, "fread() failed: %zu\n", ret);
               exit(EXIT_FAILURE);
           }

           printf("Class: %#04x\n", buffer[0]);

           fclose(fp);

           exit(EXIT_SUCCESS);
       }
