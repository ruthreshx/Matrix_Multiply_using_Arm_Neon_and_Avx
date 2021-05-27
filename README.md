# Matrix-Multiply-using-Arm-Neon

The repo is about neon based matrix multiplication and differnt datatyes like int16. int32, float32 anf float64. And the performance on raspberry pi 4 arm64 is shown along with the code.

**Requirements**

  	 1.GCC >=10
     
     
     2.Arm architecture or Qemu emulator
 
**Results**

Based on the values after running in the rasperry pi for different matrix size the graphs are obtained

![float32](https://user-images.githubusercontent.com/84854222/119775997-0161b900-bee2-11eb-8929-33c996ded71c.png)
![int16](https://user-images.githubusercontent.com/84854222/119776312-62898c80-bee2-11eb-9b45-5f0a19ede807.png)
![float64](https://user-images.githubusercontent.com/84854222/119776317-63bab980-bee2-11eb-935d-16aecdb2d595.png) 
![image](https://user-images.githubusercontent.com/84854222/119782941-5f929a00-beea-11eb-94a1-f22e869b3f00.png)



The Graph shown are ran without auto-vectorization. 
The code speed for neon arm is incresed by 10X when using auto-vectorization (eg. gcc int16mul.c -O3 int16_result). 
When testing on larger matrix the differnce between normal and neon mul is greater.

the code is checked in both raspberry pi as well in linux x86_64 using qemu emulator
to know more about qemu emulator refer this https://github.com/multiarch/qemu-user-static

It is better to use Arm Architecture instead of Qemu because the performance in Qemu is quite low since its  emulated platform. (Eg. for 1000 x 1000 float32 in qemu the time taken is 49s where as in arm architecture time is 10.4s). 

When the code is Auto-vectorized the time is reduced to is 1.4s in arm neon for 1k x 1k.

**The time chart is given below**

      Matrix size  int16(s)   int16neon(s)  int32(s)  int32neon(s) float32(s) float32neon(s) float64(s) float64neon(s) 
        8 x 8      0.000042    0.000039     0.000035   0.000022     0.000054    0.000027      0.000030    0.000027
       32 x 32     0.001439    0.001305     0.001502   0.001403     0.001559    0.000730      0.001686    0.008837
       50 x 50     0.005361    0.003472     0.005773   0.002039     0.005832    0.002198      0.005926    0.002301
      100 x 100    0.023119    0.016946     0.021783   0.007522     0.022988    0.008413      0.023099    0.011161
      200 x 200    0.158035    0.123986     0.149948   0.059083     0.159806    0.067440      0.163524    0.089471
      300 x 300    0.532597    0.435758     0.545451   0.199766     0.542208    0.228595      0.582677    0.319158
      400 x 400    1.314986    1.004088     1.367670   0.471891     1.397881    0.547197      1.725430    0.787738
      


