#include "pbm.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

//make to compile
//make cleanall to rm .o files

typedef struct {
    char mode[3];
    char argument[20];
    char infile[50];
    char outfile[50];
} Options;

int main( int argc, char *argv[] )
{
    int o;
    // for( int i=0; i<argc; i++ ){
    //     printf("Argument[%d]: %s\n", i, argv[i]);
    // }
    // printf("\n");

    //./a.out -b -g hi -i hiagain -r hihi -s -m -t hihihi -n hihihihi -o
    int bflag = 1;
    int gflag = 0;
    int iflag = 0;
    int rflag = 0;
    int sflag = 0;
    int mflag = 0;
    int tflag = 0;
    int nflag = 0;
    int oflag = 0;

    char *garg = NULL;
    char *iarg = NULL;
    char *rarg = NULL;
    char *targ = NULL;
    char *narg = NULL;
    char *oarg = NULL;

    while ((o = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1) {
        switch (o) {
        case 'b':
            bflag = 1;
            break;
                
        case 'g':
            gflag += 1;
            garg = optarg;
            bflag = 0;
            break;

        case 'i':
            iflag += 1;
            iarg = optarg;
            bflag = 0;
            break;

        case 'r':
            rflag += 1;
            rarg = optarg;
            bflag = 0;
            break;

        case 's':
            sflag += 1;
            bflag = 0;
            break;

        case 'm':
            mflag += 1;
            bflag = 0;
            break;

        case 't':
            tflag += 1;
            targ = optarg;
            bflag = 0;
            break;

        case 'n':
            nflag += 1;
            narg = optarg;
            bflag = 0;
            break;

        case 'o':
            oflag += 1;
            oarg = optarg;
            bflag = 0;
            break;

        default: // case '?'
            fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
            return 1;
            break;
        }
    }
    //Read input file
    char *infilenombre = argv[optind];
    PPMImage *readptr;
    if (infilenombre == NULL) {
        fprintf(stderr, "Error: No input file specified\n");
        return 1;
    }
    else {
        readptr = read_ppmfile(infilenombre);
        // printf("width = %d | height = %d | max = %d\n", readptr -> width, readptr -> height, readptr -> max);
    }
    //Error Handling //TODO: consider using else
    int encountered_error = 0;
    if (bflag + gflag + iflag + rflag + sflag + mflag + tflag + nflag >= 2){
        fprintf(stderr, "Error: Multiple transformations specified\n"); //throw error syntax
        encountered_error = 1;
        return 1;
    }
    if (gflag == 1){
        int gval = atoi(garg);
        if (gval < 1 || gval > 65535){ //atoi converts a string to an int
            fprintf(stderr, "Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", garg);
            encountered_error = 1;
            return 1;
        }
    }
    if (iflag == 1){
        if (strcmp(iarg,"red") != 0 && strcmp(iarg,"green") != 0 && strcmp(iarg,"blue") != 0){ //strcmp compares the two strings passed to it; if same: returns 0
            fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", iarg);
            encountered_error = 1;
            return 1;
        }
    }
    if (rflag == 1){
        if (strcmp(rarg,"red") != 0 && strcmp(rarg,"green") != 0 && strcmp(rarg,"blue") != 0){
            fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", rarg);
            encountered_error = 1;
            return 1;
        }
    }
    if (tflag == 1){
        int tval = atoi(targ);
        if (tval < 1 || tval > 8){
            fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", tval);
            encountered_error = 1;
            return 1;
        }
    }
    if (nflag == 1){
        int nval = atoi(narg);
        if (nval < 1 || nval > 8){
            fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", nval);
            encountered_error = 1;
            return 1;
        }
    }
    if (oflag == 0 || oarg == NULL){ //TODO: BONUS: throw same error if optarg of o is -b
        fprintf(stderr, "Error: No output file specified\n");
        encountered_error = 1;
        return 1;
    }
    // if (encountered_error == 1){
    //     return 1;
    // }
    
    //Bitmap
    if (bflag == 1){ 
        //create output struct
        PBMImage *ptrb;
        ptrb = new_pbmimage(readptr -> width, readptr -> height);
        //for all input pixels, update respective output pixel based on mode
        for (int wid = 0; wid < (readptr->width); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                if ((((readptr -> pixmap[0][hit][wid])+(readptr -> pixmap[1][hit][wid])+(readptr -> pixmap[2][hit][wid]))/3) < (readptr -> max /2)){
                    ptrb -> pixmap[hit][wid] = 1;
                }
                else {
                    ptrb -> pixmap[hit][wid] = 0;
                }
            }
        }
        // readptr -> pixmap[][hit][wid] //[color][height][width]
        // ptrb -> pixmap[hit][wid] //[height][width] ??
        
        //write output file
        if (oflag == 1){
            write_pbmfile(ptrb,oarg);
        }
        else {
            write_pbmfile(ptrb,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_pbmimage(ptrb);
    }
    //Grayscale
    if (gflag == 1){
        //create output struct
        PGMImage *ptrg;
        int maxg = atoi(garg);
        ptrg = new_pgmimage(readptr -> width, readptr -> height, maxg);
        //for all input pixels, update respective output pixel based on mode
        for (int wid = 0; wid < (readptr->width); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                ptrg -> pixmap[hit][wid] = ((((readptr -> pixmap[0][hit][wid])+(readptr -> pixmap[1][hit][wid])+(readptr -> pixmap[2][hit][wid]))/3.0) / (readptr -> max) * (ptrg -> max));
            }
        }
        // readptr -> pixmap[][hit][wid] //[color][height][width]
        // ptrg -> pixmap[hit][wid] //[height][width] ??
        
        //write output file
        if (oflag == 1){
            write_pgmfile(ptrg,oarg);
        }
        else {
            write_pgmfile(ptrg,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_pgmimage(ptrg);
    }

    int colorarg = -1;
    //Isolate
    if (iflag == 1){
        //create output struct
        PPMImage *ptri;
        ptri = new_ppmimage(readptr -> width, readptr -> height, readptr -> max);
        //for all input pixels, update respective output pixel based on mode
        if (strcmp(iarg,"red") == 0){
            colorarg = 0;
        }
        if (strcmp(iarg,"green") == 0){
            colorarg = 1;
        }
        if (strcmp(iarg,"blue") == 0){
            colorarg = 2;
        }
        for (int wid = 0; wid < (readptr->width); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                ptri -> pixmap[0][hit][wid] = 0;
                ptri -> pixmap[1][hit][wid] = 0;
                ptri -> pixmap[2][hit][wid] = 0;
                ptri -> pixmap[colorarg][hit][wid] = readptr -> pixmap[colorarg][hit][wid];
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptri,oarg);
        }
        else {
            write_ppmfile(ptri,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptri);
    }
    //Remove
    if (rflag == 1){
        //create output struct
        PPMImage *ptrr;
        ptrr = new_ppmimage(readptr -> width, readptr -> height, readptr -> max);
        //for all input pixels, update respective output pixel based on mode
        if (strcmp(rarg,"red") == 0){
            colorarg = 0;
        }
        if (strcmp(rarg,"green") == 0){
            colorarg = 1;
        }
        if (strcmp(rarg,"blue") == 0){
            colorarg = 2;
        }
        for (int wid = 0; wid < (readptr->width); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                ptrr -> pixmap[0][hit][wid] = readptr -> pixmap[0][hit][wid];
                ptrr -> pixmap[1][hit][wid] = readptr -> pixmap[1][hit][wid];
                ptrr -> pixmap[2][hit][wid] = readptr -> pixmap[2][hit][wid];
                ptrr -> pixmap[colorarg][hit][wid] = 0;
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptrr,oarg);
        }
        else {
            write_ppmfile(ptrr,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptrr);
    }
    //Sepia
    if (sflag == 1){
        //create output struct
        PPMImage *ptrs;
        ptrs = new_ppmimage(readptr -> width, readptr -> height, readptr -> max);
        //for all input pixels, update respective output pixel based on mode
        double oldR = -1;
        double oldG = -1;
        double oldB = -1;
        for (int wid = 0; wid < (readptr->width); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                oldR = readptr -> pixmap[0][hit][wid];
                oldG = readptr -> pixmap[1][hit][wid];
                oldB = readptr -> pixmap[2][hit][wid];
                ptrs -> pixmap[0][hit][wid] = (0.393*oldR)+(0.769*oldG)+(0.189*oldB); //newR
                ptrs -> pixmap[1][hit][wid] = (0.349*oldR)+(0.686*oldG)+(0.168*oldB); //newG
                ptrs -> pixmap[2][hit][wid] = (0.272*oldR)+(0.534*oldG)+(0.131*oldB); //newB
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptrs,oarg);
        }
        else {
            write_ppmfile(ptrs,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptrs);
    }
    //Mirror
    if (mflag == 1){
        //create output struct
        PPMImage *ptrm;
        ptrm = new_ppmimage(readptr -> width, readptr -> height, readptr -> max);
        //for all input pixels, update respective output pixel based on mode
        for (int wid = 0; wid < ((readptr->width)/2); wid++){
            for (int hit = 0; hit < (readptr->height); hit++){
                //Original Half:
                ptrm -> pixmap[0][hit][wid] = readptr -> pixmap[0][hit][wid];
                ptrm -> pixmap[1][hit][wid] = readptr -> pixmap[1][hit][wid];
                ptrm -> pixmap[2][hit][wid] = readptr -> pixmap[2][hit][wid];
                //Mirrored Half:
                ptrm -> pixmap[0][hit][(readptr->width) - wid] = readptr -> pixmap[0][hit][wid];
                ptrm -> pixmap[1][hit][(readptr->width) - wid] = readptr -> pixmap[1][hit][wid];
                ptrm -> pixmap[2][hit][(readptr->width) - wid] = readptr -> pixmap[2][hit][wid];
                //TODO: determine if line down the middle is intendid or not
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptrm,oarg);
        }
        else {
            write_ppmfile(ptrm,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptrm);
    }
    //Thumbnail
    if (tflag == 1){
        int n = atoi(targ);
        //create output struct
        PPMImage *ptrt;
        ptrt = new_ppmimage( ((readptr -> width)%n == 0) ? ((readptr -> width)/n) : (((readptr -> width)/n) + 1), ((readptr -> height)%n == 0) ? ((readptr -> height)/n) : (((readptr -> height)/n) + 1), readptr -> max); // (condition) ? (true) : (false)
        //for all input pixels, update respective output pixel based on mode
        for (int wid = 0; wid < ((readptr->width)/n); wid++){
            for (int hit = 0; hit < ((readptr->height)/n); hit++){
                ptrt -> pixmap[0][hit][wid] = readptr -> pixmap[0][hit * n][wid * n];
                ptrt -> pixmap[1][hit][wid] = readptr -> pixmap[1][hit * n][wid * n];
                ptrt -> pixmap[2][hit][wid] = readptr -> pixmap[2][hit * n][wid * n];
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptrt,oarg);
        }
        else {
            write_ppmfile(ptrt,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptrt);
    }
    //Nup
    if (nflag == 1){
        int n = atoi(narg);
        //create output struct
        PPMImage *ptrn;
        ptrn = new_ppmimage(readptr -> width, readptr -> height, readptr -> max);
        //for all input pixels, update respective output pixel based on mode
        int shifth = (readptr -> height)/n;
        int shiftw = (readptr -> width)/n;
        for (int wid = 0; wid < ((readptr->width)/n); wid++){
            for (int hit = 0; hit < ((readptr->height)/n); hit++){
                for (int shiftMultiplierw = 0; shiftMultiplierw < n; shiftMultiplierw++){
                    for (int shiftMultiplierh = 0; shiftMultiplierh < n; shiftMultiplierh++){
                        ptrn -> pixmap[0][hit + (shifth * shiftMultiplierh)][wid + (shiftw * shiftMultiplierw)] = readptr -> pixmap[0][hit * n][wid * n];
                        ptrn -> pixmap[1][hit + (shifth * shiftMultiplierh)][wid + (shiftw * shiftMultiplierw)] = readptr -> pixmap[1][hit * n][wid * n];
                        ptrn -> pixmap[2][hit + (shifth * shiftMultiplierh)][wid + (shiftw * shiftMultiplierw)] = readptr -> pixmap[2][hit * n][wid * n];
                    }
                }
            }
        }
        //write output file
        if (oflag == 1){
            write_ppmfile(ptrn,oarg);
        }
        else {
            write_ppmfile(ptrn,"a.out");
        }
        //destroy all image structs and any other allocated memory
        del_ppmimage(ptrn);
    }

    // printf("flag vals = %d  %d  %d  %d  %d  %d  %d  %d  \n\n", bflag, gflag, iflag, rflag, sflag, mflag, tflag, nflag);
    return 0;
}
