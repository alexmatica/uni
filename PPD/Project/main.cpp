#include <iostream>
#include "my_png.h"
#include <mpi.h>


using namespace std;

int procId;
int nrProcs;
int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
};

#define NEIGHBORHOOD 1


void master(MyPng *myPng) {
    MPI_Status status;

    int width = myPng->getWidth();
    int height = myPng->getHeight();
    for(int i=0; i<height; i++)
        for(int j=0; j<width * 4; j++)
            myPng->copy[i][j] = 0;

    cout << "Zerorized output\n" << flush;

    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i=1; i<nrProcs; i++){
        int begin = height * width * i / nrProcs;
        int end = height * width * (i+1) / nrProcs;
        MPI_Send(&begin, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
    }

    for(int i=0; i<height; i++){
        MPI_Bcast(myPng->row_pointers[i], width, MPI_INT, 0, MPI_COMM_WORLD);
    }

    cout << "Sent all needed stuff to children\n" << flush;

    for (int i=0; i< width * height / nrProcs; i++){
        int x = i / width;
        int y = i % width;

        int l = 0, c = 0;
        int accumulator[4] = {0,0,0,0};
        int count = 0;

        for (int m = x - NEIGHBORHOOD; m <= x + NEIGHBORHOOD; m++) {
            for (int n = y - NEIGHBORHOOD; n <= y + NEIGHBORHOOD; n++)
                if (m < 0 || n < 0 || m >= height || n >= width) {
                    c++;
                    continue;
                }
                else {
                    count ++;
                    png_bytep pixel = &myPng->row_pointers[m][n * 4];
                    for (int p = 0; p < 4; p++)
                        accumulator[p] += pixel[p];
                    c++;
                }
            l++;
        }
        png_bytep pixel = &myPng->copy[x][y*4];
        for(int p=0; p<4; p++)
            pixel[p] = accumulator[p] / count;
    }

    cout << "Master done processing...\n" << flush;

    auto slave_row = (png_bytep)malloc(sizeof(png_byte) * 4 * width);
    for (int s = 1; s < nrProcs; s++){
        for (int i=0; i<height; i++){
            MPI_Recv(slave_row, width, MPI_INT, s, 3, MPI_COMM_WORLD, &status);
            for (int j=0; j <width * 4; j++){
                myPng->copy[i][j] += slave_row[j];
            }
        }
    }

    cout << "Received all modifications from children\n" << flush;
    free(slave_row);
}

void slave(){
    int width, height;
    MPI_Status status;

    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

    png_bytep *input;
    png_bytep *output;

    input = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i=0; i<height; i++)
        input[i] = (png_bytep)malloc(sizeof(png_bytep) * 4 * width);

    output = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i=0; i<height; i++) {
        output[i] = (png_bytep) malloc(sizeof(png_bytep) * 4 * width);
        for (int j = 0; j < width * 4; j++)
            output[i][j] = 0;
    }

    int begin, end;
    MPI_Recv(&begin, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

    for (int i=0; i<height; i++)
        MPI_Bcast(input[i], width, MPI_INT, 0, MPI_COMM_WORLD);

    cout << "Slave received all info from master\n" << flush;

    for (int i=begin; i < end; i++){
        int x = i / width;
        int y = i % width;

        int l = -1, c = 0;
        int accumulator[4] = {0,0,0,0};
        int count = 0;

        for (int m = x - NEIGHBORHOOD; m <= x + NEIGHBORHOOD; m++) {
            l++;
            for (int n = y - NEIGHBORHOOD; n <= y + NEIGHBORHOOD; n++)
                if (m < 0 || n < 0 || m >= height || n >= width) {
                    c++;
                    continue;
                }
                else {
                    count ++;
                    png_bytep pixel = &input[m][n*4];
                    for (int p=0; p<4; p++)
                        accumulator[p] += pixel[p];
                    c++;
                }
        }
        png_bytep pixel = &output[x][y*4];
        for (int p=0; p<4; p++)
            pixel[p] = accumulator[p] / count;
    }

    for (int i=0; i<height; i++){
        MPI_Send(output[i], width, MPI_INT, 0, 3, MPI_COMM_WORLD);
        free(input[i]);
        free(output[i]);
    }

    free(input);
    free(output);

}

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    MPI_Comm_size(MPI_COMM_WORLD, &nrProcs);

    if (procId == 0){
        if (argc != 3){
            cout << "Usage: ./main <png_in_file> <png_out_file>\n";
            exit(1);
        }
        MyPng myPng(argv[1]);
        master(&myPng);
        cout << "Master finished successfully\n" << flush;
        myPng.write_png(argv[2]);
    } else{
        slave();
    }

    MPI_Finalize();
    return 0;
}