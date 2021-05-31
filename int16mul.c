
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <arm_neon.h>

#define BLOCK_SIZE 32 // mention the Matrix size here 


void matrix_multiply_c(int16_t *A, int16_t *B, int16_t *C, uint32_t n, uint32_t m, uint32_t k) {
        clock_t t;
        t=clock();
        for (int i_idx=0; i_idx<n; i_idx++) {
                for (int j_idx=0; j_idx<m; j_idx++) {
                        C[n*j_idx + i_idx] = 0;
                        for (int k_idx=0; k_idx<k; k_idx++) {
                                C[n*j_idx + i_idx] += A[n*k_idx + i_idx]*B[k*j_idx + k_idx];
                        }
                }
        }
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
        printf("The program took %f seconds to execute normal mul\n", time_taken);
}

void matrix_multiply_neon(int16_t  *A, int16_t  *B, int16_t *C, uint32_t n, uint32_t m, uint32_t k) {
        /* 
         * Multiply matrices A and B, store the result in C. 
         * It is the user's responsibility to make sure the matrices are compatible.
         */     
        clock_t t;
        t=clock();
        int A_idx;
        int B_idx;
        int C_idx;
        
        // these are the columns of a 4x4 sub matrix of A
        int16x8_t A0;
        int16x8_t A1;
        int16x8_t A2;
        int16x8_t A3,A4,A5,A6,A7;
        
        // these are the columns of a 4x4 sub matrix of B
        int16x8_t B0;
        int16x8_t B1;
        int16x8_t B2;
        int16x8_t B3,B4,B5,B6,B7;
        
        // these are the columns of a 4x4 sub matrix of C
        int16x8_t C0;
        int16x8_t C1;
        int16x8_t C2;
        int16x8_t C3;
        int16x8_t C4;
        int16x8_t C5;
        int16x8_t C6;
        int16x8_t C7;

        for (int i_idx=0; i_idx<n; i_idx+=8) {
                for (int j_idx=0; j_idx<m; j_idx+=8) {
                        // Zero accumulators before matrix op
                        C0 = vmovq_n_s16(0);
                        C1 = vmovq_n_s16(0);
                        C2 = vmovq_n_s16(0);
                        C3 = vmovq_n_s16(0);
                        C4 = vmovq_n_s16(0);
                        C5 = vmovq_n_s16(0);
                        C6 = vmovq_n_s16(0);
                        C7 = vmovq_n_s16(0);
                        for (int k_idx=0; k_idx<k; k_idx+=8) {
                                A_idx = i_idx + n*k_idx;
                                B_idx = k*j_idx + k_idx;
                                
                                A0 = vld1q_s16(A+A_idx);
                                A1 = vld1q_s16(A+A_idx+n);
                                A2 = vld1q_s16(A+A_idx+2*n);
                                A3 = vld1q_s16(A+A_idx+3*n);
                                A4 = vld1q_s16(A+A_idx+4*n);
                                A5 = vld1q_s16(A+A_idx+5*n);
                                A6 = vld1q_s16(A+A_idx+6*n);
                                A7 = vld1q_s16(A+A_idx+7*n);
                                
                                B0 = vld1q_s16(B+B_idx);
                                C0 = vmlaq_laneq_s16(C0, A0, B0,0);
                                C0 = vmlaq_laneq_s16(C0, A1, B0,1);
                                C0 = vmlaq_laneq_s16(C0, A2, B0,2);
                                C0 = vmlaq_laneq_s16(C0, A3, B0,3);
                                C0 = vmlaq_laneq_s16(C0, A4, B0,4);
                                C0 = vmlaq_laneq_s16(C0, A5, B0,5);
                                C0 = vmlaq_laneq_s16(C0, A6, B0,6);
                                C0 = vmlaq_laneq_s16(C0, A3, B0,7);

                                B1 = vld1q_s16(B+B_idx+k);
                                C1 = vmlaq_laneq_s16(C1, A0, B1,0);
                                C1 = vmlaq_laneq_s16(C1, A1, B1,1);
                                C1 = vmlaq_laneq_s16(C1, A2, B1,2);
                                C1 = vmlaq_laneq_s16(C1, A3, B1,3);
                                C1 = vmlaq_laneq_s16(C1, A4, B1,4);
                                C1 = vmlaq_laneq_s16(C1, A5, B1,5);
                                C1 = vmlaq_laneq_s16(C1, A6, B1,6);
                                C1 = vmlaq_laneq_s16(C1, A7, B1,7);

                                B2 = vld1q_s16(B+B_idx+2*k);
                                C2 = vmlaq_laneq_s16(C2, A0, B2,0);
                                C2 = vmlaq_laneq_s16(C2, A1, B2,1);
                                C2 = vmlaq_laneq_s16(C2, A2, B2,2);
                                C2 = vmlaq_laneq_s16(C2, A3, B2,3);
                                C2 = vmlaq_laneq_s16(C2, A4, B2,4);
                                C2 = vmlaq_laneq_s16(C2, A5, B2,5);
                                C2 = vmlaq_laneq_s16(C2, A6, B2,6);
                                C2 = vmlaq_laneq_s16(C2, A7, B2,7);
                                
                                B3 = vld1q_s16(B+B_idx+3*k);
                                C3 = vmlaq_laneq_s16(C3, A0, B3,0);
                                C3 = vmlaq_laneq_s16(C3, A1, B3,1);
                                C3 = vmlaq_laneq_s16(C3, A2, B3,2);
                                C3 = vmlaq_laneq_s16(C3, A3, B3,3);
                                C3 = vmlaq_laneq_s16(C3, A4, B3,4);
                                C3 = vmlaq_laneq_s16(C3, A5, B3,5);
                                C3 = vmlaq_laneq_s16(C3, A6, B3,6);
                                C3 = vmlaq_laneq_s16(C3, A7, B3,7);

                                B4 = vld1q_s16(B+B_idx+4*k);
                                C4 = vmlaq_laneq_s16(C4, A0, B4,0);
                                C4 = vmlaq_laneq_s16(C4, A1, B4,1);
                                C4 = vmlaq_laneq_s16(C4, A2, B4,2);
                                C4 = vmlaq_laneq_s16(C4, A3, B4,3);
                                C4 = vmlaq_laneq_s16(C4, A4, B4,4);
                                C4 = vmlaq_laneq_s16(C4, A5, B4,5);
                                C4 = vmlaq_laneq_s16(C4, A6, B4,6);
                                C4 = vmlaq_laneq_s16(C4, A7, B4,7);

                                B5 = vld1q_s16(B+B_idx+5*k);
                                C5 = vmlaq_laneq_s16(C5, A0, B5,0);
                                C5 = vmlaq_laneq_s16(C5, A1, B5,1);
                                C5 = vmlaq_laneq_s16(C5, A2, B5,2);
                                C5 = vmlaq_laneq_s16(C5, A3, B5,3);
                                C5 = vmlaq_laneq_s16(C5, A4, B5,4);
                                C5 = vmlaq_laneq_s16(C5, A5, B5,5);
                                C5 = vmlaq_laneq_s16(C5, A6, B5,6);
                                C5 = vmlaq_laneq_s16(C5, A7, B5,7);

                                B6 = vld1q_s16(B+B_idx+6*k);
                                C6 = vmlaq_laneq_s16(C6, A0, B6,0);
                                C6 = vmlaq_laneq_s16(C6, A1, B6,1);
                                C6 = vmlaq_laneq_s16(C6, A2, B6,2);
                                C6 = vmlaq_laneq_s16(C6, A3, B6,3);
                                C6 = vmlaq_laneq_s16(C6, A4, B6,4);
                                C6 = vmlaq_laneq_s16(C6, A5, B6,5);
                                C6 = vmlaq_laneq_s16(C6, A6, B6,6);
                                C6 = vmlaq_laneq_s16(C6, A7, B6,7);

                                B7 = vld1q_s16(B+B_idx+7*k);
                                C7 = vmlaq_laneq_s16(C7, A0, B7,0);
                                C7 = vmlaq_laneq_s16(C7, A1, B7,1);
                                C7 = vmlaq_laneq_s16(C7, A2, B7,2);
                                C7 = vmlaq_laneq_s16(C7, A3, B7,3);
                                C7 = vmlaq_laneq_s16(C7, A4, B7,4);
                                C7 = vmlaq_laneq_s16(C7, A5, B7,5);
                                C7 = vmlaq_laneq_s16(C7, A6, B7,6);
                                C7 = vmlaq_laneq_s16(C7, A7, B7,7);
                        }
                        // Compute base index for stores
                        C_idx = n*j_idx + i_idx;
                        vst1q_s16(C+C_idx, C0);
                        vst1q_s16(C+C_idx+n, C1);
                        vst1q_s16(C+C_idx+2*n, C2);
                        vst1q_s16(C+C_idx+3*n, C3);
                        vst1q_s16(C+C_idx+4*n, C4);
                        vst1q_s16(C+C_idx+5*n, C5);
                        vst1q_s16(C+C_idx+6*n, C6);
                        vst1q_s16(C+C_idx+7*n, C7);
                }
        }
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
        printf("The program took %f seconds to execute neon mul\n", time_taken);
}



void print_matrix(int16_t *M, uint32_t cols, uint32_t rows) {
        for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++) {
                        printf("%d ", M[j*rows + i]);
                }
                printf("\n");
        }
        printf("\n");
}

void matrix_init_rand(int16_t *M, uint32_t numvals) {
        for (int i=0; i<numvals; i++) {
                //M[i] = (uint)rand()/(uint)(RAND_MAX);
                M[i]=5;
        }
}


int main() {
        uint32_t n = 2*BLOCK_SIZE; // rows in A
        uint32_t m = 2*BLOCK_SIZE; // cols in B
        uint32_t k = 2*BLOCK_SIZE; // cols in a and rows in b
        
        static int16_t A[4096]; //A[BLOCK_SIZE*4]
        static int16_t B[4096];
        static int16_t D[4096];
        static int16_t E[4096];
        
        matrix_init_rand(A, n*k);
        matrix_init_rand(B, k*m);

        print_matrix(A, k, n);
        print_matrix(B, m, k);
        matrix_multiply_c(A, B, E, n, m, k);
        printf("normal mul\n");
        print_matrix(E, n, m);
        
        printf("Neon\n");
        matrix_multiply_neon(A, B, D, n, m, k);
        print_matrix(D, n, m);

}
