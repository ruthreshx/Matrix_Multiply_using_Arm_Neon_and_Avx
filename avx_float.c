#include <xmmintrin.h> //Header for intrinsic functions
#include <nmmintrin.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>
int main()
{
int MAX_DIM = 5000; 
static float a[5000][5000] __attribute__ ((aligned(16)));
static float b[5000][5000] __attribute__ ((aligned(16)));
static float c[5000][5000] __attribute__ ((aligned(16)));
static float d[5000][5000] __attribute__ ((aligned(16)));
static float e[5000][5000] __attribute__ ((aligned(16)));
for (int i = 0; i < MAX_DIM; ++i)
{
	for (int j = 0; j < MAX_DIM; ++j)
	{
		a[i][j] = 13; // Arbitrary initializations - Replace to test your multiplication
		b[i][j] = 22; // Arbitrary initializations - Replace to test your multiplication
		c[i][j] = 0; // Necessary Initialization - Don't change
		d[i][j] = 0; // Necessary Initialization - Don't change
                e[i][j] =0;
	}
}
// Unoptimized Matrix Multiplication
clock_t Time1 = clock();
for (int i = 0; i < MAX_DIM; ++i)
{
	for (int j = 0; j < MAX_DIM; ++j)
	{
		for (int k = 0; k < MAX_DIM; k++)
		{
            c[i][j] += a[i][k] * b[k][j];
        }
    }
}
clock_t Time2 = clock();
  
clock_t Time3 = clock();

for(int i=0;i<MAX_DIM;i+=1){

      for(int j=0;j<MAX_DIM;j+=4){

        for(int k=0;k<MAX_DIM;k+=4){

          __m128 result = _mm_load_ps(&d[i][j]);

          __m128 a_line  = _mm_load_ps(&a[i][k]);

          __m128 b_line0 = _mm_load_ps(&b[k][j+0]);

          __m128 b_line1 = _mm_loadu_ps(&b[k][j+1]);

          __m128 b_line2 = _mm_loadu_ps(&b[k][j+2]);

          __m128 b_line3 = _mm_loadu_ps(&b[k][j+3]);

         result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0x00), b_line0));
         result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0x55), b_line1));
         result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0xaa), b_line2));
         result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0xff), b_line3));
         _mm_store_ps(&d[i][j],result);
        }
      }
    }

clock_t Time4 = clock();

clock_t Time5 = clock();

for(int i=0;i<MAX_DIM;i+=1){

      for(int j=0;j<MAX_DIM;j+=8){

        for(int k=0;k<MAX_DIM;k+=8){

          __m256 result = _mm256_load_ps(&e[i][j]);

          __m256 a_line  = _mm256_load_ps(&a[i][k]);

          __m256 b_line0 = _mm256_load_ps(&b[k][j+0]);

          __m256 b_line1 = _mm256_loadu_ps(&b[k][j+1]);

          __m256 b_line2 = _mm256_loadu_ps(&b[k][j+2]);

          __m256 b_line3 = _mm256_loadu_ps(&b[k][j+3]);

          __m256 b_line4 = _mm256_loadu_ps(&b[k][j+4]);

          __m256 b_line5 = _mm256_loadu_ps(&b[k][j+5]);

          __m256 b_line6 = _mm256_loadu_ps(&b[k][j+6]);

          __m256 b_line7 = _mm256_loadu_ps(&b[k][j+7]);

         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0x00), b_line0));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0x55), b_line1));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0xaa), b_line2));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0xff), b_line3));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0x22), b_line4));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0x77), b_line5));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0xcc), b_line6));
         result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_shuffle_ps(a_line, a_line, 0x99), b_line3));
         _mm256_store_ps(&e[i][j],result);
        }
      }
    }

clock_t Time6 = clock();
// Calculate and print execution times
double TotalTimeLoop = ((double) Time2 - (double) Time1) / CLOCKS_PER_SEC;
double TotalTimeSIMD = ((double) Time4 - (double) Time3) / CLOCKS_PER_SEC;
double TotalTime256SIMD = ((double) Time6 - (double) Time5) / CLOCKS_PER_SEC;
//printf("c mat\n");
//for(int i=0;i<MAX_DIM;i++){
//        for(int j=0;j<MAX_DIM;j++){
//            printf("%f ",c[i][j]);
//        }
//        printf("\n");
 //   }
//printf("d mat\n");
//for(int i=0;i<MAX_DIM;i++){
//        for(int j=0;j<MAX_DIM;j++){
 //           printf("%f ",d[i][j]);
  //      }
   //     printf("\n");
    //}
//printf("e mat\n");
//for(int i=0;i<MAX_DIM;i++){
  //      for(int j=0;j<MAX_DIM;j++){
    //        printf("%f ",e[i][j]);
      //  }
   //     printf("\n");
   // }
printf("Increase in Performance using _m128 is %.7f s\n", TotalTimeLoop/TotalTimeSIMD);
printf("Increase in Performance using _m256 is %.7f s\n", TotalTimeLoop/TotalTime256SIMD);
printf(" Time taken by loop is %.7f s \n", TotalTimeLoop);
printf(" Time taken by SIMD optimized code is %.7f s\n", TotalTimeSIMD);
printf(" Time taken by SIMD256 optimized code is %.7f s\n", TotalTime256SIMD);
return 0;
}

