/*
 * @Description: 简化JSON格式字段对照表
 * @Author: Auto Generated
 * @Date: 2025-08-24
 * @Version: 1.0
 */

#ifndef __JSON_FIELD_MAPPING_H
#define __JSON_FIELD_MAPPING_H

/* 
 * 简化JSON格式字段映射
 * 目的：减少传输数据大小，提高效率
 */

// 时间相关字段
#define JSON_TIMESTAMP_KEY      "t"        // 原: "timestamp"
#define JSON_SEQUENCE_KEY       "seq"      // 原: "seq" (保持不变)

// IMU传感器字段
#define JSON_ACC_X_KEY          "ax"       // 原: imu.acc[0]
#define JSON_ACC_Y_KEY          "ay"       // 原: imu.acc[1] 
#define JSON_ACC_Z_KEY          "az"       // 原: imu.acc[2]
#define JSON_GYRO_X_KEY         "gx"       // 原: imu.gyro[0]
#define JSON_GYRO_Y_KEY         "gy"       // 原: imu.gyro[1]
#define JSON_GYRO_Z_KEY         "gz"       // 原: imu.gyro[2]
#define JSON_TEMPERATURE_KEY    "temp"     // 原: imu.temp (保持不变)

// 超声波传感器字段
#define JSON_DISTANCE_KEY       "dist"     // 原: ultrasonic.distance
#define JSON_VALID_KEY          "ok"       // 原: valid (单独数据包用)
#define JSON_IMU_VALID_KEY      "imu_ok"   // IMU数据有效性 (组合包用)
#define JSON_DIST_VALID_KEY     "dist_ok"  // 超声波数据有效性 (组合包用)

// 机器人状态字段
#define JSON_GAIT_MODE_KEY      "mode"     // 原: robot_mode.mode_id
#define JSON_MAIN_STATE_KEY     "state"    // 原: robot_mode.state_id
#define JSON_ROBOT_STATUS_KEY   "status"   // 原: data.robot_status (保持不变)
#define JSON_UPTIME_KEY         "uptime"   // 原: data.uptime (保持不变)

// 系统状态字段
// #define JSON_BATTERY_KEY        "bat"      // 原: status.battery (已删除 - 功能无效)

/* 数据包大小优化统计 */
#define ORIGINAL_IMU_BUFFER_SIZE        256     // 原始IMU缓冲区大小
#define SIMPLIFIED_IMU_BUFFER_SIZE      192     // 简化后IMU缓冲区大小

#define ORIGINAL_ULTRASONIC_BUFFER_SIZE 128     // 原始超声波缓冲区大小
#define SIMPLIFIED_ULTRASONIC_BUFFER_SIZE 96    // 简化后超声波缓冲区大小

#define ORIGINAL_COMBINED_BUFFER_SIZE   512     // 原始组合数据包缓冲区大小
#define SIMPLIFIED_COMBINED_BUFFER_SIZE 256     // 简化后组合数据包缓冲区大小

#define ORIGINAL_STATUS_BUFFER_SIZE     256     // 原始状态数据缓冲区大小
#define SIMPLIFIED_STATUS_BUFFER_SIZE   128     // 简化后状态数据缓冲区大小

/* 字段数据类型说明 */
/*
 * t (timestamp): uint32_t - 系统时间戳(毫秒)
 * seq: uint32_t - 数据包序列号
 * ax,ay,az: float - 加速度(m/s²), 精度3位小数
 * gx,gy,gz: float - 角速度(°/s), 精度1位小数  
 * temp: float - 温度(°C), 精度1位小数
 * dist: float - 距离(cm), 精度1位小数
 * ok: uint8_t - 数据有效性 (0=无效, 1=有效) - 单独数据包用
 * imu_ok: uint8_t - IMU数据有效性 (0=无效, 1=有效) - 组合包用  
 * dist_ok: uint8_t - 超声波数据有效性 (0=无效, 1=有效) - 组合包用
 * mode: int - 步态模式ID (枚举值)
 * state: int - 主状态ID (枚举值)
 * status: uint8_t - 机器人状态码
 * uptime: uint32_t - 系统运行时间(毫秒)
 * 注意: 电池电量字段已删除(功能无效)
 */

#endif /* __JSON_FIELD_MAPPING_H */
