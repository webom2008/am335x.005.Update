#ifndef CSERIALANALYSIS_H
#define CSERIALANALYSIS_H

#include "../Utilities/canalysisdata.h"

class CModuleBase;
class CDataModel;

class CSerialAnalysis
{
public:
    enum KEY_TYPE_IDENTIFIER_CODE{
        KEY_BUTTON_IDENTIFIER_ID            = 0x01,
        KEY_ENCODER_IDENTIFIER_ID,
        KEY_SELFTEST_ID,
        KEY_ALARM_LIGHT_ID
    };

    enum KEY_BUTTON_IDENTIFIER_CODE{
        KEY_BUTTON_RESET_ID                 =   0X01,
        KEY_BUTTON_ALARMPAUSE_ID            =   0x02,
        KEY_BUTTON_COMBINATION_ID           =   0x03,
        KEY_BUTTON_WAVEFROZEN_ID            =   0x04,
        KEY_BUTTON_RECORD_ID                =   0x08,
        KEY_BUTTON_NIBP_ID                  =   0x10,
        KEY_BUTTON_MAINMENU_ID              =   0x20
    };

    enum KEY_ENCODER_IDENTIFIER_CODE{
        KEY_ENCODER_NEXT_ID                 =   0x01,
        KEY_ENCODER_PREVIOUS_ID             =   0X02,
        KEY_ENCODER_PRESS_ID                =   0X04,
    };

    enum BCK_IDENTIFIER_CODE{
        BCK_SHUTDOWN_ID                     =   0X01,
        BCK_BATTERY_INFO_ID,
        BCK_VOLTAGE_INFO_ID,
        BCK_ANALOG_OUTPUT_ID,
        BCK_ENABLED_RECORDER_ID,
        BCK_LOW_BATTERY_ALAEM_ID,
        BCK_SYSTEM_RESET_ID
    };

    enum AIO_IDENTIFIER_CODE{
        INVALID_IDENTIFIER_ID               =   0X00,   //无效ID
        AIO_VERSION_ID                      =   0xE0,   //模块版本信息
        AIO_POST_RESULT_ID,                             //开机自检结果
        AIO_SELFTEST_RESULT_ID,                         //常规自检结果
        AIO_ABNORMAL_RESET_ID,                          //异常复位标记包
        AIO_PATIENT_TYPE_ID,                            //病人类型
        AIO_WORK_MODE_ID,                               //工作模式
        AIO_PARAM_REQUEST_ID,                           //AIO参数请求
        ECG_RT_DATA_ID                      =   0xA0,   //ECG实时测量数据包
        ECG_POLARIZATION_VOLTAGE_ID,                    //ECG极化电压信息包
        ECG_HR_RESP_RR_ID,                              //心率HR和呼吸率RR
        ECG_ARRHYTHMIA_ID,                              //心律失常分析结果数据包
        ECG_PVCS_ID,                                    //每分钟室早个数
        ECG_ST_OFFSET_VALUE_ID,                         //ST段偏移值
        ECG_NORMAL_ALARM_ID,                            //心电常规报警
        ECG_LOAD_ECG_INFO_ID                =   0xA7,   //导联连接和心电信息
        ECG_OVERLOAD_INFO_ID,                           //心电过载报警信息
        ECG_ARRHYTHMIA_CHANL_ID,                        //心律分析通道选择
        ECG_ANALYSIS_STATUS_ID              =   0xAB,   //心电分析状态
        ECG_HB_INFO_ID,                                 //心搏信息
        ECG_CHANEL_SELECTION_ID             =   0x20,   //导联通道选择
        ECG_ST_TEMPLATE_DATA_ID,                        //ST段模板数据
        ECG_DEBUG_INFO_ID                   =   0x22,   //ECG算法调试信息
        ECG_ARRHYTHMIA_SELFLEARN_ID         =   0x23,   //触发心率失常自动学习
        ECG_ST_ANALYSIS_SWITCH_ID           =   0x25,   //ST段分析开关
        ECG_ARRHYTHMIA_SWITCH_ID,                       //心率失常分析开关
        ECG_ST_PARAM_ID,                                //ST测量设置
        ECG_FILTER_SELECTION_ID,                        //工频陷波器选择
        ECG_CALIBRATION_SIG_ID,                         //心电校准信号设置
        ECG_LEAD_TYPE_SELECTION_ID,                     //心电导连类型
        ECG_PACE_SWITCH_ID,                             //PACE开关
        ECG_PACE_LEAD_SELECTION_ID,                     //PACE检测导连选择
        ECG_PACE_REJECT_SWITCH_ID,                      //PACE过冲抑制开关
        ECG_DEBUG_ID                        =   0x2F,   //ECG调试接口
        RESP_RT_DATA_ID                     =   0xB0,   //RESP实时数据包
        RESP_APNEA_ID,                                  //呼吸窒息报警信息B1H
        RESP_CVA_HIZ_ALARM_ID,                          //CVA和呼吸高阻报警信息数据包
        RESP_OTHER_ALARM,                               //呼吸其他报警
        RESP_CALCULATION_METHOD_ID          =   0x31,   //呼吸计算方式
        RESP_LEAD_SELECTION_ID,                         //呼吸导联选择
        RESP_CARRIER_SWITCH_ID,                         //呼吸载波开关
        RESP_APNEA_TIME_ID,                             //窒息报警时间
        RESP_DEBUG_ID                       =   0x3E,   //RESP配置调试接口
        TEMP_DATA_ID                        =   0xB8,   //体温上传数据包
        TEMP_ALARM_ID,                                  //体温报警
        TEMP_DEBUG_ID                       =   0x3F,   //TEMP配置调试接口
        NIBP_RESULT_ID                      =   0xC0,   //NIBP测量结果
        NIBP_RT_DATA_ID,                                //NIBP实时数据包 100Hz
        NIBP_CALIBRATION_STATUS_ID,                     //NIBP校准状态
        NIBP_ALARM_ID                       =   0xC4,   //NIBP报警
        NIBP_RT_DATA2_ID                    =   0xC5,   //NIBP实时数据包 10Hz
        NIBP_COUNTDOWN_ID                   =   0xC7,   //NIBP倒计时数据包 1Hz
        NIBP_MEASURMENT_ID                  =   0x40,   //NIBP测量
        NIBP_STOP_ID,                                   //NIBP停止
        NIBP_CALIBRATION_MODE_ID            =   0x43,   //NIBP校准模式
        NIBP_CALIBRATION_PROCESS_ID,                    //NIBP校准过程
        NIBP_OVP_CALIBRATION_ID,                        //NIBP过压保护校准
        NIBP_RESET_ID,                                  //NIBP复位
        NIBP_PRECHARGE_PRESSURE_ID,                     //NIBP预充压力
        NIBP_STATIC_PRESSURE_TEST_ID,                   //静态压力检验
        NIBP_VENIPUNCTURE_ID                =   0x49,   //NIBP辅助静脉穿刺
        NIBP_LEAKTEST_ID                    =   0x4D,   //NIBP漏气检测
        NIBP_DEBUG_ID                       =   0x4F,   //NIBP调试接口
        SPO2_VERSION_ID                     =   0x10,   //SPO2模块版本信息
        SPO2_ALARM_ID,                                  //SPO2报警信息
        SPO2_SELFTEST_ID,                               //SPO2自检结果
        SPO2_PATIENT_TYPE_ID,                           //SPO2病人类型和响应速度
        SPO2_WORKMODE_ID,                               //SPO2工作模式查询和设置命令
        SPO2_LOW_POWER_ID                   =   0x16,   //SPO2进入低功耗模式
        SPO2_POWER_DETECT_ID,                           //SPO2电源检测信息上传包
        SPO2_RAW_DATA_ID                    =   0X19,   //SPO2算法测试模式下实时数据上传包
        SPO2_NORMALIZED_DATA_ID,                        //SPO2归一化数据上传包
        SPO2_RESULT_ID,                                 //SPO2计算结果上传包
        SPO2_DEBUG_ID                       =   0x1F,   //SPO2调试接口
        SF_SPO2_UPDATE                      =   0xD1,
        SF_AIO_STM_UPDATE                   =   0xD2,
        SF_AIO_DSP_UPDATE                   =   0xD3,
        SF_BACK_UPDATE                      =   0xD4,
        SF_RECORD_UPDATE                    =   0xD5,
        SF_EXPAND_UPDATE                    =   0xD6,
    };

    enum RECORSER_IDENTIFIER_CODE{
        RECORDER_SELF_TEST                  =   0x01,
        RECORDER_STATUS                     =   0x02,

        RECORDER_REQUEST                    =   0x0E,
        RECORDER_END_TASK                   =   0x0F,

        RECORDER_BITMAP_DATA                =   0x10,
        RECORDER_WAVE_DATA                  =   0x20,

        RECORDER_BITMAP_48_8_NO_GRID        =   0x11,
        RECORDER_BITMAP_48_8_GRID_25MM      =   0x12,
        RECORDER_BITMAP_48_8_GRID_50MM      =   0x13,
        RECORDER_BITMAP_16_24_NO_GRID_25MM  =   0x14,
        RECORDER_BITMAP_16_24_NO_GRID_50MM  =   0x15,

        RECORDER_WAVE_TASK_25_1             =   0x21,
        RECORDER_WAVE_TASK_25_2             =   0x22,
        RECORDER_WAVE_TASK_25_3             =   0x23,
        RECORDER_WAVE_TASK_25_4             =   0x24,

        RECORDER_WAVE_TASK_50_1             =   0x28,
        RECORDER_WAVE_TASK_50_2             =   0x29,
        RECORDER_WAVE_TASK_50_3             =   0x2A,
        RECORDER_WAVE_TASK_50_4             =   0x2B,

        RECORDER_ECG_GAIN_POS               =   0x2F
    };

public:
    explicit CSerialAnalysis();
    ~CSerialAnalysis();

public:
    virtual void processCommStatus(const unsigned long long &sysTick) = 0;
    virtual void try_Parse(CModuleBase *pModule,
                           const CAnalysisData &rxData,
                           const unsigned long long &sysTick) = 0;

};
#endif // CSERIALANALYSIS_H
