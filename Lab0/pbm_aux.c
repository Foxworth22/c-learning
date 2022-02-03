#include "pbm.h"
#include <stdlib.h>
#include <stdio.h>

struct tripint {
    int w;
    int h;
    int m;
};

struct duoint {
    int w;
    int h;
};

//@31:30
PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m ) //3D
{
    //return allocated storage amounts using malloc()
    PPMImage *ptr; //create pointer
    ptr = malloc(sizeof(PPMImage)); //create new PPMImage
    ptr -> height = h;
    ptr -> width = w;
    ptr -> max = m;

    for (int x=0; x<3; x++){
        ptr -> pixmap[x] = malloc( w * sizeof(double*) );
    }
    //allocate array of h unsigned int POINTERS
    //allocate each array pointed to by ptrs in h to an array of doubles
    for (int x=0; x<3; x++){
        ptr -> pixmap[x] = malloc( h * sizeof(double *) ); //sizeof int pointers //(unsigned int **) malloc //TODO: added *
        for (int i=0; i<h; i++){
            ptr -> pixmap[x][i] = malloc( w * sizeof(double) );// consider making int float for Sepia //(unsigned int *) malloc
        }
    }
    return ptr;// does this include pixmap correctly?
}

PBMImage * new_pbmimage( unsigned int w, unsigned int h ) //2D
{
    //return allocated storage amounts using malloc()
    PBMImage *ptr; //create pointer
    ptr = malloc(sizeof(PBMImage)); //create new PBMImage
    ptr -> height = h;
    ptr -> width = w;
    //allocate array of h unsigned int POINTERS
    //allocate each array pointed to by ptrs in h to an array of unsigned ints
    ptr -> pixmap = malloc( h * sizeof(unsigned int *) ); //sizeof int pointers //(unsigned int **) malloc
    for (int i=0; i<h; i++){
        ptr -> pixmap[i] = malloc( w * sizeof(unsigned int) );//TODO: consider making int float for Sepia //(unsigned int *) malloc
    }

    return ptr;

}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m ) //2D
{
    //return allocated storage amounts using malloc()
    PGMImage *ptr; //create pointer
    ptr = malloc(sizeof(PGMImage)); //create new PGMImage
    ptr -> height = h;
    ptr -> width = w;
    ptr -> max = m;
    //allocate array of h unsigned int POINTERS
    //allocate each array pointed to by ptrs in h to an array of unsigned ints
    ptr -> pixmap = malloc( h * sizeof(unsigned int *) ); //sizeof int pointers //(unsigned int **) malloc
    for (int i=0; i<h; i++){
        ptr -> pixmap[i] = malloc( w * sizeof(unsigned int) );// consider making int float for Sepia //(unsigned int *) malloc
    }

    return ptr;// does this include pixmap correctly?
}

void del_ppmimage( PPMImage * p ) //3D
{
    //deletes allocated structure using free()
    //printf("%u",p -> height);
    for (int f = 0; f < (p -> height); f++){
        free(p -> pixmap[0][f]);
        free(p -> pixmap[1][f]);
        free(p -> pixmap[2][f]);
    }
    free(p -> pixmap[0]);
    free(p -> pixmap[1]);
    free(p -> pixmap[2]);
    free(p);
}

void del_pgmimage( PGMImage * p ) //2D
{
    //deletes allocated structure using free()
    for (int f = 0; f < (p -> height); f++){
        free(p -> pixmap[f]);
    }
    free(p -> pixmap);
    free(p);
}

void del_pbmimage( PBMImage * p ) //2D
{
    //deletes allocated structure using free()
    for (int f = 0; f < (p -> height); f++){
        free(p -> pixmap[f]);
    }
    free(p -> pixmap);
    free(p);
}

