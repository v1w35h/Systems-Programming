## assembly language example

.section  .text
.global asmtest
asmtest:
    #for (i=0; i <  100000;i++)
    li t0, 0
    li t1, 100000
for_loop_start:
    bge     t0, t1,  for_loop_end      #if iterator is 100000 or up terminate

    addi    t0, t0, 1
    j       for_loop_start
for_loop_end:
    mv      a0, t0
    ret