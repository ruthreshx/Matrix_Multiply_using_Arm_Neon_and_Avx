# Matrix-Multiply-using-Arm-Neon

The repo is about neon based matrix multiplication and differnt datatyes like int16. int32, float32 anf float64. And the performance on raspberry pi 4 arm64 is shown along with the code.

**Requirements**

  	 1.GCC >=10
     
     
     2.Arm architecture or Qemu emulator
 
**Results**

Based on the values after running in the rasperry pi for different matrix sixe the graphs are obtained

![float32](https://user-images.githubusercontent.com/84854222/119775997-0161b900-bee2-11eb-8929-33c996ded71c.png)
![int16](https://user-images.githubusercontent.com/84854222/119776312-62898c80-bee2-11eb-9b45-5f0a19ede807.png)
![float64](https://user-images.githubusercontent.com/84854222/119776317-63bab980-bee2-11eb-935d-16aecdb2d595.png) 
![image](https://user-images.githubusercontent.com/84854222/119782941-5f929a00-beea-11eb-94a1-f22e869b3f00.png)



The Graph shown are ran without auto-vectorization. 
The code speed for neon arm is incresed by 10X when using auto-vectorization (eg. gcc int16mul.c -O3 int16_result). 
When testing on larger matrix the differnce between normal and neon mul is greater.

the code is checked in both raspberry pi as well in linux x86_64 using qemu emulator
to know more about qemu emulator refer this https://github.com/multiarch/qemu-user-static

It ia better to use Arm Architecture instead of Qemu because the performance in Qemu is low when to arm. (eg. for 1000 x 1000 float32 time taken is 49s in qemu where as in arm time is 10.4s). When the code is Auto-vectorized the time taken is 1.4s 


