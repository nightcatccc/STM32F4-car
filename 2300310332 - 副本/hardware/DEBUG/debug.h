/**
 ****************************************************************************************************
 * @file        debug.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       pid上位机调试 代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __DEBUG_H
#define __DEBUG_H

#include "sys.h"

#define DEBUG_ENABLE       1         /* PID调试使能 */
#define EN_CRC             1         /* CRC校验，0：关闭，1：开启 */

#define DEBUG_DATA_HEAD   0xC5       /* 帧头 */
#define DEBUG_DATA_END    0x5C       /* 帧尾 */


/*********************************** 开发板 ——> 上位机 *******************************************/

/* 数据类别 */
typedef enum
{
    TYPE_ERROR          = 0x0F,      /* 故障类型 */
    TYPE_STATUS         = 0x10,      /* 设备状态 */
    TYPE_SPEED          = 0x11,      /* 速度 */
    TYPE_HAL_ENC        = 0x12,      /* 霍尔、编码器位置值 */
    TYPE_VBUS           = 0x13,      /* 电压 */
    TYPE_AMP            = 0x14,      /* 电流 */
    TYPE_TEMP           = 0x15,      /* 驱动板温度 */
    TYPE_SUM_LEN        = 0x16,      /* 总里程 */
    TYPE_BEM            = 0x17,      /* 反电动势 */
    TYPE_MOTOR_CODE     = 0x18,      /* 电机类型 */
    TYPE_TORQUE         = 0x19,      /* 扭矩 */
    TYPE_POWER          = 0x1A,      /* 功率 */
    TYPE_PID1           = 0x20,      /* 用户PID1参数上报 */
    TYPE_PID2           = 0x21,      /* PID2 */
    TYPE_PID3           = 0x22,      /* PID3 */
    TYPE_PID4           = 0x23,      /* PID4 */
    TYPE_PID5           = 0x24,      /* PID5 */
    TYPE_PID6           = 0x25,      /* PID6 */
    TYPE_PID7           = 0x26,      /* PID7 */
    TYPE_PID8           = 0x27,      /* PID8 */
    TYPE_PID9           = 0x28,      /* PID9 */
    TYPE_PID10          = 0x29,      /* PID10 */
    TYPE_USER_DATA      = 0x30,      /* 波形数据上报 */
}upload_type;

/* 故障类型 */
typedef enum
{
    HALL_ENC_ERROR      = 0x01,      /* 编码器、霍尔错误 */
    OVERSPEED_ERROR     = 0x02,      /* 电机过速 */
    DB_OVERTEMP_ERROR   = 0x04,      /* 驱动板过温 */
    M_OVERTEMP_ERROR    = 0x08,      /* 电机过温 */
    DB_OVERVOL_ERROR    = 0x10,      /* 驱动板过压 */
    DB_UNDERVOL_ERROR   = 0x10,      /* 驱动板欠压 */
    DB_OVERCUR_ERROR    = 0x10,      /* 驱动板过流 */
    UNKNOWN_ERROR       = 0x10,      /* 未知错误 */
}motor_error;

/* 电机状态 */
typedef enum
{
    IDLE_STATE          = 0x00,      /* 空闲状态 */
    RUN_STATE           = 0x01,      /* 运行状态 */
    ERROR_STATE         = 0x02,      /* 错误状态 */
    LRTOUT_STATE        = 0x03,      /* 堵转超时 */
    BREAKED_STATE       = 0x04,      /* 刹车 */
}motor_state;

/* 电机类型 */
typedef enum
{
    DC_MOTOR            = 0x10,      /* 直流有刷电机 */
    BLDC_MOTOR          = 0x11,      /* 直流无刷电机 */
    PMSM_MOTOR          = 0x12,      /* 永磁同步电机 */
    STEP_MOTOR          = 0x13,      /* 步进电机 */
    SERVO_MOTOR         = 0x14,      /* 伺服电机 */
    EXCHANG_MOTOR       = 0x15,      /* 变频器(三相交流异步电机) */
    HELM_MOTOR          = 0x16,      /* 舵机 */
}motor_code;

/* PID参数存放结构体 */
typedef struct
{
    union
    {
        float pidf[3];               /* PID参数发送存放 */
        int8_t pidi8[12];            /* PID参数接收存放 */
    }pid;
}pid_struct;

/* 参数存放结构体（开发板——>上位机） */
typedef struct
{
    uint8_t status;                  /* 电机状态 */
    int16_t speed;                   /* 电机速度 */
    uint8_t hall_p;                  /* 霍尔位置值 */
    uint16_t encode_p;               /* 编码器位置值 */
    float bus_vol;                   /* 电压 */
    float amp[3];                    /* 电流 */
    float temp[2];                   /* 温度 */
    uint64_t sum_len;                /* 总里程 */
    float bem[3];                    /* 反电动势 */
    uint8_t motor_code;              /* 电机类型 */
    float torque;                    /* 扭矩 */
    float power;                     /* 功率 */
    pid_struct pid[10];              /* 10组PID参数（收发共用） */
    int16_t user_data[16];           /* 波形数据 */
}debug_data;

extern debug_data g_debug;           /* 发送变量 */


/*********************************** 上位机  ——>  开发板 *******************************************/

/* 数据类别 */
typedef enum
{
    CMD_GET_ALL_DATA    = 0x19,      /* 获取全部参数 */
    CMD_SET_CTR_CODE    = 0x21,      /* 下发控制指令 */
    CMD_SET_CTR_MODE    = 0x22,      /* 下发控制模式 */
    CMD_SET_SPEED       = 0x23,      /* 设定速度 */
    CMD_SET_TORQUE      = 0x24,      /* 设定转矩 */
    CMD_SET_VF_VOL      = 0x25,      /* 设定VF电压 */
    CMD_SET_VF_IF_FRE   = 0x26,      /* 设定V/F、IF频率 */
    CMD_SET_IF_CUR      = 0x27,      /* 设定IF电流 */
    CMD_SET_DQ_CUR_D    = 0x28,      /* 设定DQ电流D */
    CMD_SET_DQ_CUR_Q    = 0x29,      /* 设定DQ电流Q */
    CMD_SET_PID1        = 0x31,      /* 设定PID1参数 */
    CMD_SET_PID2        = 0x32,      /* PID2 */
    CMD_SET_PID3        = 0x33,      /* PID3 */
    CMD_SET_PID4        = 0x34,      /* PID4 */
    CMD_SET_PID5        = 0x35,      /* PID5 */
    CMD_SET_PID6        = 0x36,      /* PID6 */
    CMD_SET_PID7        = 0x37,      /* PID7 */
    CMD_SET_PID8        = 0x38,      /* PID8 */
    CMD_SET_PID9        = 0x39,      /* PID9 */
    CMD_SET_PID10       = 0x3A,      /* PID10 */
}cmd_type;

/* 下发控制指令 */
typedef enum
{
    HALT_CODE           = 0x01,      /* 停机 */
    RUN_CODE            = 0x02,      /* 运行 */
    BREAKED             = 0x03,      /* 刹车 */
}cmd_code;

/* 下发控制模式 */
typedef enum
{
    SPEED_MODE          = 0x01,      /* 转速模式 */
    TORQUE_MODE         = 0x02,      /* 转矩模式 */
    IF_MODE             = 0x03,      /* IF模式 */
    VF_MODE             = 0x04,      /* VF模式 */
    DQ_MODE             = 0x05,      /* DQ模式 */
}cmd_mode;

/* 参数存放结构体（上位机——>开发板） */
typedef struct
{
    uint8_t Ctrl_code;
    uint8_t Ctrl_mode;
    float *speed;
    float *torque;
    float pid[3];
}debug_data_rev;

extern debug_data_rev debug_rev;     /* 接收变量 */

#define DEBUG_REV_MAX_LEN   17       /* 接收数据最大长度 */

/*****************************************************************************************************/

/* 底层驱动函数 */
void debug_obj_init(debug_data *data);                                              /* 内存初始化 */
void debug_handle(uint8_t *data);                                                   /* 上位机数据解析 */
void debug_upload_data(debug_data * data, uint8_t upload_type);                     /* 数据上传 */

/* 应用层函数 */
void debug_init(void);                                                              /* 初始化调试 */
void debug_set_point_range(float max_limit,float min_limit,float step_max);         /* 设置目标速度范围 */

void debug_send_initdata(upload_type PIDx,float *SetPoint,float P,float I,float D); /* PID初始化数据上传 */
void debug_send_current(float U_I,float V_I,float W_I);                             /* 电流数据上传 */
void debug_send_valtage(float valtage);                                             /* 电压数据上传 */
void debug_send_power(float power);                                                 /* 功率数据上传 */
void debug_send_speed(float speed);                                                 /* 速度数据上传 */
void debug_send_distance(uint64_t len);                                             /* 总里程数据上传 */
void debug_send_temp(float motor_temp,float board_temp);                            /* 温度数据上传 */
void debug_send_motorstate(motor_state motor_codestae);                             /* 电机状态上传 */
void debug_send_motorcode(motor_code motorcode);                                    /* 电机类型上传 */
void debug_send_wave_data(uint8_t chx,int16_t wave);                                /* 波形数据上传 */

void debug_receive_pid(upload_type PIDx,float *P,float *I,float *D);                /* 上位机PID参数接收 */
uint8_t debug_receive_ctrl_code(void);                                              /* 上位机命令接收 */

#endif


