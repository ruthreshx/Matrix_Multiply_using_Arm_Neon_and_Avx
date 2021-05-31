
#include <xmmintrin.h> //Header for intrinsic functions
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>
#include <smmintrin.h> 

int main()
{
// Variables definition and initialization
int MAX_DIM = 5000;
static int a[25000000] ;
static int b[25000000] ;
static int c[25000000] ;
static int d[25000000] ;
static int e[25000000] ;
//printf("size = %d \n",MAX_DIM);
int n= MAX_DIM*MAX_DIM;
for (int i = 0; i <MAX_DIM; ++i)
{
	for (int j = 0; j < MAX_DIM; ++j){
		a[i*MAX_DIM+j] = 10; 
		b[i*MAX_DIM+j] = 15; 
		c[i*MAX_DIM+j] = 0; 
		d[i*MAX_DIM+j] = 0; 
                e[i*MAX_DIM+j] = 0;
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
            c[i*MAX_DIM+j] += a[i*MAX_DIM+k] * b[k*MAX_DIM+j];
        }
    }
}
clock_t Time2 = clock();

clock_t Time3 = clock();


for(int i=0;i<MAX_DIM;i+=1){
  
      for(int j=0;j<MAX_DIM;j+=4){
     
        for(int k=0;k<MAX_DIM;k+=4){
         
          __m128i result = _mm_load_si128((__m128i*)&d[i*MAX_DIM+j]);
          
          __m128i a_line  = _mm_load_si128((__m128i*)&a[i*MAX_DIM+k]);
          
          __m128i b_line0 = _mm_load_si128((__m128i*)&b[k*MAX_DIM+j+0]);
        
          __m128i b_line1 = _mm_load_si128((__m128i*)&b[k*MAX_DIM+j+1]);
        
          __m128i b_line2 = _mm_load_si128((__m128i*)&b[k*MAX_DIM+j+2]);
     
          __m128i b_line3 = _mm_load_si128((__m128i*)&b[k*MAX_DIM+j+3]);

         result = _mm_add_epi32(result, _mm_mullo_epi32(a_line, b_line0));
         result = _mm_add_epi32(result, _mm_mullo_epi32(a_line, b_line1));
         result = _mm_add_epi32(result, _mm_mullo_epi32(a_line, b_line2));
         result = _mm_add_epi32(result, _mm_mullo_epi32(a_line, b_line3));
         _mm_store_si128((__m128i*)&d[i*MAX_DIM+j],result);
        }
      }
    }
// YOUR CODE HERE
clock_t Time4 = clock();

clock_t Time5 = clock();

for(int i=0;i<MAX_DIM;i+=1){
      
      for(int j=0;j<MAX_DIM;j+=8){
        
        for(int k=0;k<MAX_DIM;k+=8){
          
          __m256i result = _mm256_load_si256((__m256i*)&e[i*MAX_DIM+j]);
          __m256i a_line  = _mm256_load_si256((__m256i*)&a[i*MAX_DIM+k]);    
          __m256i b_line0 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+0]);
          __m256i b_line1 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+1]);
          __m256i b_line2 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+2]);
          __m256i b_line3 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+3]);
          __m256i b_line4 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+4]);
          __m256i b_line5 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+5]);
          __m256i b_line6 = _mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+6]);
          __m256i b_line7 =_mm256_load_si256((__m256i*)&b[k*MAX_DIM+j+7]);
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line0));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line1));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line2));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line3));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line4));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line5));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line6));
         result = _mm256_add_epi32(result, _mm256_mullo_epi32(a_line, b_line7));
         _mm256_store_si256((__m256i*)&e[i*MAX_DIM+j],result);
        }
      }
    }

clock_t Time6 = clock();
// printf("c mat\n");
// for(int i=0;i<MAX_DIM;i++){
//         for(int j=0;j<MAX_DIM;j++){
//             printf("%d ",c[i*MAX_DIM+j]);
//         }
//         printf("\n");
//     }
// printf("d mat\n");
// for(int i=0;i<MAX_DIM;i++){
//         for(int j=0;j<MAX_DIM;j++){
//             printf("%d ",d[i*MAX_DIM+j]);
//         }
//         printf("\n");
// }
// printf("e mat\n");
// for(int i=0;i<MAX_DIM;i++){
//         for(int j=0;j<MAX_DIM;j++){
//             printf("%d ",e[i*MAX_DIM+j]);
//         }
//         printf("\n");
//}

// Calculate and print execution times
double TotalTimeLoop = ((double) Time2 - (double) Time1) / CLOCKS_PER_SEC;
double TotalTimeSIMD = ((double) Time4 - (double) Time3) / CLOCKS_PER_SEC;
double TotalTime256SIMD = ((double) Time6 - (double) Time5) / CLOCKS_PER_SEC;
printf("Performance gain is %.7f \n", TotalTimeLoop/TotalTimeSIMD);
printf("Performance gain is %.7f \n", TotalTimeLoop/TotalTime256SIMD);
printf(" Time taken by loop is %.7f \n", TotalTimeLoop);
printf(" Time taken by SIMD optimized code is %.7f \n", TotalTimeSIMD);
printf(" Time taken by SIMD 256 optimized code is %.7f \n", TotalTime256SIMD);
// printf("%f %f %f ",TotalTimeLoop,TotalTimeSIMD,TotalTime256SIMD);

return 0;
}
