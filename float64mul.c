#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <arm_neon.h>
#include <time.h>
#define BLOCK_SIZE 375 

void matrix_multiply_c(float64_t *A, float64_t *B, float64_t *C, uint32_t n, uint32_t m, uint32_t k) {
    clock_t t;
   	t = clock();
  	for (int i_idx=0; i_idx<n; i_idx++) {
                for (int j_idx=0; j_idx<m; j_idx++) {
                       // ite++;
                        C[n*j_idx + i_idx] = 0;
                        for (int k_idx=0; k_idx<k; k_idx++) {
                              //  ite++;
                                C[n*j_idx + i_idx] += A[n*k_idx + i_idx]*B[k*j_idx + k_idx];
                        }
                }
        }
  	t = clock() - t;
   	double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
  	printf("The program took for normal mul is %f seconds to execute", time_taken);
}

void matrix_multiply_neon(float64_t  *A, float64_t  *B, float64_t *C, uint32_t n, uint32_t m, uint32_t k) {
      
        int A_idx;
        int B_idx;
        int C_idx;
        float64x2_t A0;
        float64x2_t A1;
        
        float64x2_t B0;
        float64x2_t B1;
        
        float64x2_t C0;
        float64x2_t C1;

        for (int i_idx=0; i_idx<n; i_idx+=2) {
                
                for (int j_idx=0; j_idx<m; j_idx+=2) {

                        C0 = vmovq_n_f64(0);
                        C1 = vmovq_n_f64(0);
                        for (int k_idx=0; k_idx<k; k_idx+=2) {
                                
                                A_idx = i_idx + n*k_idx;
                                B_idx = k*j_idx + k_idx;
                                // Load most current A values in row
                                A0 = vld1q_f64(A+A_idx);
                                A1 = vld1q_f64(A+A_idx+n);
                  
                                B0 = vld1q_f64(B+B_idx);
                                C0 = vfmaq_laneq_f64(C0, A0, B0, 0);
                                C0 = vfmaq_laneq_f64(C0, A1, B0, 1);
                            
                                B1 = vld1q_f64(B+B_idx+k);
                                C1 = vfmaq_laneq_f64(C1, A0, B1, 0);
                                C1 = vfmaq_laneq_f64(C1, A1, B1, 1);
                               
                        }
                        // Compute base index for stores
                        C_idx = n*j_idx + i_idx;
                        vst1q_f64(C+C_idx, C0);
                        vst1q_f64(C+C_idx+n, C1);

                }
        }
       
}

void print_matrix(float64_t *M, uint32_t cols, uint32_t rows) {
        for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++) {
                        printf("%f ", M[j*rows + i]);
                }
                printf("\n");
        }
        printf("\n");
}
void matrix_init_rand(float64_t *M, uint32_t numvals) {
        for (int i=0; i<numvals; i++) {
               M[i] = (float)rand()/(float)(RAND_MAX);
        }
}

int main() {
        uint32_t n = 2*BLOCK_SIZE; // rows in A
        uint32_t m = 2*BLOCK_SIZE; // cols in B
        uint32_t k = 2*BLOCK_SIZE; // cols in a and rows in b
        static float64_t A[562500]; //A[BLOCK_SIZE*4]
        static float64_t B[562500];
        static float64_t C[562500];
        static float64_t D[562500];
        static float64_t E[562500];
        
        matrix_init_rand(A, n*k);
        matrix_init_rand(B, k*m);
        matrix_init(C, n, m, 0);
        print_matrix(A, k, n);
        print_matrix(B, m, k);
        
        matrix_multiply_c(A, B, E, n, m, k);
        printf("normal mul\n");
        print_matrix(E, n, m);
        printf("===============================\n");
        clock_t t;
      	t = clock();
        matrix_multiply_neon(A, B, D, n, m, k);
        t = clock() - t;
      	double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
      	printf("The program took for neon %f seconds to execute", time_taken);
	      printf("Neon\n");
        print_matrix(D, n, m);
        return 0;
}
