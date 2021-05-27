# Matrix-Multiply-using-Arm-Neon

The code are tested using raspberry pi 4 arm64

**Requirements**

  	 1.GCC 10 or GCC 10+
     
     
     2.Arm or Qemu emulator
 
**Results**

Based on the values after running in the rasperry pi for different matrix sixe the graphs are obtained

![float32](https://user-images.githubusercontent.com/84854222/119775997-0161b900-bee2-11eb-8929-33c996ded71c.png)
![int32](https://user-images.githubusercontent.com/84854222/119776307-60bfc900-bee2-11eb-921c-efd087b40dac.png)
![int16](https://user-images.githubusercontent.com/84854222/119776312-62898c80-bee2-11eb-9b45-5f0a19ede807.png)
![float64](https://user-images.githubusercontent.com/84854222/119776317-63bab980-bee2-11eb-935d-16aecdb2d595.png)


The code speed is incresed by 10X when using auto-vectorization (eg. gcc int16mul.c -O3 int16_result)

the code is checked in both raspberry pi as well in linux x86_64 using qemu emulator
to know more about qemu emulator refer this https://github.com/multiarch/qemu-user-static


