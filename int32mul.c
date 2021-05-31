#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <arm_neon.h>
#include <time.h>
#define BLOCK_SIZE 32


void matrix_multiply_c(int32_t *A, int32_t *B, int32_t *C, uint32_t n, uint32_t m, uint32_t k) {
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

void matrix_multiply_neon(int32_t  *A, int32_t  *B, int32_t *C, uint32_t n, uint32_t m, uint32_t k) {
        /* 
         * Multiply matrices A and B, store the result in C. 
         * It is the user's responsibility to make sure the matrices are compatible.
         */     

        int A_idx;
        int B_idx;
        int C_idx;

        int32x4_t A0;
        int32x4_t A1;
        int32x4_t A2;
        int32x4_t A3;

        int32x4_t B0;
        int32x4_t B1;
        int32x4_t B2;
        int32x4_t B3;

        int32x4_t C0;
        int32x4_t C1;
        int32x4_t C2;
        int32x4_t C3;
        clock_t t;
        t=clock();
        for (int i_idx=0; i_idx<n; i_idx+=4) {
                for (int j_idx=0; j_idx<m; j_idx+=4) {
                        
                        C0 = vmovq_n_s32(0);
                        C1 = vmovq_n_s32(0);
                        C2 = vmovq_n_s32(0);
                        C3 = vmovq_n_s32(0);
                        for (int k_idx=0; k_idx<k; k_idx+=4) {
                                /
                                A_idx = i_idx + n*k_idx;
                                B_idx = k*j_idx + k_idx;
                                //A0 = vld1q_s32(arr1);
                                //A0 = vtrn1q_s32(A0, A0);
                                
                                A0 = vld1q_s32(A+A_idx);
                                A1 = vld1q_s32(A+A_idx+n);
                                A2 = vld1q_s32(A+A_idx+2*n);
                                A3 = vld1q_s32(A+A_idx+3*n);
                               
                                B0 = vld1q_s32(B+B_idx);
                                C0 = vmlaq_laneq_s32(C0, A0, B0,0);
                                C0 = vmlaq_laneq_s32(C0, A1, B0,1);
                                C0 = vmlaq_laneq_s32(C0, A2, B0,2);
                                C0 = vmlaq_laneq_s32(C0, A3, B0,3);
                                
                                B1 = vld1q_s32(B+B_idx+k);
                                C1 = vmlaq_laneq_s32(C1, A0, B1,0);
                                C1 = vmlaq_laneq_s32(C1, A1, B1,1);
                                C1 = vmlaq_laneq_s32(C1, A2, B1,2);
                                C1 = vmlaq_laneq_s32(C1, A3, B1,3);
                                
                                B2 = vld1q_s32(B+B_idx+2*k);
                                C2 = vmlaq_laneq_s32(C2, A0, B2,0);
                                C2 = vmlaq_laneq_s32(C2, A1, B2,1);
                                C2 = vmlaq_laneq_s32(C2, A2, B2,2);
                                C2 = vmlaq_laneq_s32(C2, A3, B2,3);
                                
                                B3 = vld1q_s32(B+B_idx+3*k);
                                C3 = vmlaq_laneq_s32(C3, A0, B3,0);
                                C3 = vmlaq_laneq_s32(C3, A1, B3,1);
                                C3 = vmlaq_laneq_s32(C3, A2, B3,2);
                                C3 = vmlaq_laneq_s32(C3, A3, B3,3);
                        }
                        
                        C_idx = n*j_idx + i_idx;
                        vst1q_s32(C+C_idx, C0);
                        vst1q_s32(C+C_idx+n, C1);
                        vst1q_s32(C+C_idx+2*n, C2);
                        vst1q_s32(C+C_idx+3*n, C3);
                }
        }
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
        printf("The program took %f seconds to execute Neon mul\n", time_taken);
}



void print_matrix(int32_t *M, uint32_t cols, uint32_t rows) {
        for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++) {
                        printf("%d ", M[j*rows + i]);
                }
                printf("\n");
        }
        printf("\n");
}

void matrix_init_rand(int32_t *M, uint32_t numvals) {
        for (int i=0; i<numvals; i++) {
                M[i]=i+1; // make use of rand function to create random number if needed 
        }
}



int main() {
        uint32_t n = 2*BLOCK_SIZE; // rows in A
        uint32_t m = 2*BLOCK_SIZE; // cols in B
        uint32_t k = 2*BLOCK_SIZE; // cols in a and rows in b
        
        static int32_t A[4096]; //A[BLOCK_SIZE*4]
        static int32_t B[4096];
        static int32_t D[4096];
        static int32_t E[4096];

        matrix_init_rand(A, n*k);
        matrix_init_rand(B, k*m);
        print_matrix(A, k, n);
        print_matrix(B, m, k);
                
        matrix_multiply_c(A, B, E, n, m, k);
        printf("C\n");
        print_matrix(E, n, m);     
        matrix_multiply_neon(A, B, D, n, m, k);
        printf("Neon\n");
        print_matrix(D, n, m);
        return 0;
}
