#include "Inverse_K.h"
#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f                       // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                    // half the sample period   采样周期的一半

// 全局变量声明
extern float a[3], w[3], angle[3];

void Gyroscopetest(void);

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

floatRPY MUP6050_Data_Process(void);

void IMU_Attitude_Control(void);

void IMU_Init(void);
