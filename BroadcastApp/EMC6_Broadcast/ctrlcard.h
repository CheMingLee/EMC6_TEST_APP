//Ctrl Command  (0x0000), bit 15: 0 Ctrl, 1 List; Bit 14: Feedback; Bit 13: Error code
//Feedback
#define CMD_GET_LIST_SIZE							0x4000
#define CMD_RS232_TEST								0x4001
#define CMD_GET_LABEL								0x4002
#define CMD_GET_COUNT								0x4003
#define CMD_GET_HEAD_STATUS							0x4004
#define CMD_GET_HEX_VERSION							0x4005
#define CMD_GET_HI_DATA								0x4006
#define CMD_GET_INPUT_POINTER						0x4007
#define CMD_GET_IO_STATUS							0x4008
#define CMD_GET_LIST_SPACE							0x4009
#define CMD_GET_CARD_VERSION						0x400a
#define CMD_GET_SERIAL_NUMBER						0x400b
#define CMD_GET_STARTSTOP_INFO						0x400c
#define CMD_GET_STATUS								0x400d
#define CMD_GET_TIME								0x400e
#define CMD_GET_VALUE								0x400f
#define CMD_GET_WAIT_STATUS							0x4010
#define CMD_GET_WAVEFORM							0x4011
#define CMD_GET_XY_POS								0x4012
#define CMD_MEASUREMENT_STATUS						0x4013
#define CMD_READ_IO_PORT							0x4014
#define CMD_READ_PIXEL_AD							0x4015
#define CMD_READ_STATUS								0x4016
#define CMD_GET_MOTION_PULSE_COUNT					0x4017
#define CMD_GET_MOTION_INPOS						0x4018
#define CMD_GET_MOTION_STATUS						0x4019
#define CMD_GET_MOTION_ENCODER						0x401a
#define CMD_GET_MOTION_SENSOR						0x401b
#define CMD_DEBUG_WATCH_DOG							0x401c
#define CMD_MOTION_ARC_GET_SIZE						0x401d
//
#define CMD_GET_IO_STATUS_EX						0x401f
#define CMD_AUTO_CAL								0x4020
#define CMD_CONTROL_COMMAND							0x4021
#define CMD_GET_ENCODER_SPEED						0x4022
#define CMD_SET_HW_CFG								0x4023
#define CMD_GET_HW_CFG								0x4024
#define CMD_ERASE_FLASH								0x4025
#define CMD_READ_MEM								0x4026
#define CMD_MEM_TO_QSPI								0x4027
#define CMD_QSPI_TO_MEM								0x4028
//Sha204
#define CMD_SHA204_WAKEUP							0x4029
#define CMD_SHA204_SLEEP							0x402a
#define CMD_SHA204_EXECUTE_CMD						0x402b
#define CMD_SHA204_GET_RESPONSE						0x402c
#define CMD_GET_MOTION_INDEX_STATUS					0x402d
#define CMD_GET_CORRECT_GRID						0x402e //Bill_20160804 add correct grid return
#define CMD_GET_DNA_CODE							0x402f //Bill_20160830 add CARD FPGA DNA return
#define CMD_SET_DNA_CODE							0x4030 //Bill_20160906 新增DNA更新命令
//Bill_20161117 Add Encoder Pos Trigger Mode
#define CMD_GET_ENCODER_POS_TRIGGER					0x4031
//Bill_20170314 Add ETab io commands
#define CMD_GET_ETAB_TRG_ENCODER					0x4032
#define CMD_GET_ETAB_REC_ENCODER					0x4033
#define CMD_GET_SELECT_LIST							0x4034 //Bill_20170808 調整為多List架構
//Bill_20170925 四軸同動整合調整修改
#define CMD_MOTION_ARC_GET_SIZE_EX					0x4035
//Bill_20171019 四軸同動新增時間紀錄命令
#define CMD_GET_RECORD_MARK_TIME					0x4036
//Bill_20181221 新增四軸同動(AMOF)振鏡與軸空位置紀錄資料
#define CMD_GET_RECORD_AMOF_DIFF					0x4037
//Bill_20190312 修改支援特殊條件NG Jump處理
#define CMD_GET_NG_RECORD							0x4038
//Bill_20191028 支援IP IPG YLPM設定
#define CMD_GET_IPG_PD_READ							0x4039
//Bill_20200601 支援內頻狀態檢查
#define CMD_GET_INT_FREQ_STATUS						0x403a
//Bill_20200904 支援ECM-SK EtherCAT卡控制
#define CMD_GET_ECM_SK_DATA							0x403b
#define CMD_GET_ECM_SK_DATA_EX						0x403c
// cmleex_20210419 新增TCP/IP傳輸方式
#define CMD_GET_IP_ADDRESS							0x403d

//No Feedback
#define CMD_AUTO_CHANGE								0x0000
#define CMD_AUTO_CHANGE_POS							0x0001
#define CMD_DISABLE_LASER							0x0002
#define CMD_ENABLE_LASER							0x0003
#define CMD_EXECUTE_AT_POINTER						0x0004
#define CMD_GOTO_XY									0x0005
#define CMD_HOME_POSITION							0x0006
#define CMD_LOAD_PROGRAM_FILE						0x0007
#define CMD_QUIT_LOOP								0x0008
#define CMD_RELEASE_WAIT							0x0009
#define CMD_RESTART_LIST							0x000a
#define CMD_SELECT_COR_TABLE						0x000b
#define CMD_SELECT_LIST								0x000c
#define CMD_SELECT_RTC								0x000d
#define CMD_SET_CONTROL_MODE						0x000e
#define CMD_SET_DELAY_MODE							0x000f
#define CMD_SET_INPUT_POINTER						0x0010
#define CMD_SET_LIST_MODE							0x0011
#define CMD_SET_MAX_COUNT							0x0012
#define CMD_SET_PISO_CONTROL						0x0013
#define CMD_SET_SOFTSTART_LEVEL						0x0014
#define CMD_SOFTSTART_MODE							0x0015
#define CMD_START_LOOP								0x0016
//#define CMD_STOP_EXCUTION							0x0017
#define CMD_WRITE_8BIT_PORT							0x0018
#define CMD_WRITE_DA_X								0x0019
#define CMD_WRITE_IO_PORT							0x001a
#define CMD_Z_OUT									0x001b
//#define CMD_AUT_CHANGE								0x001c
//#define CMD_LOAD_PRO								0x001d
#define CMD_SET_FLYSPEED_UPDATE_TIME				0x001e
#define CMD_SET_PWM_VARPERIOD						0x001f
#define CMD_SET_STANDBY								0x0020
#define CMD_LASER_SIGNAL_ON							0x0021
#define CMD_SET_LASER_FPK							0x0022
#define CMD_SET_LASER_MODE							0x0023
#define CMD_SET_MATRIX								0x0024
#define CMD_SET_OFFSET								0x0025
#define CMD_MOTION_MOVETO							0x0026
#define CMD_SET_MOTION_PARAM1						0x0027
#define CMD_SET_MOTION_PARAM2						0x0028
#define CMD_SET_MOTION_PARAM3						0x0029
#define CMD_SET_MOTION_XYTABLE						0x002a
#define CMD_SET_MOTION_HOME							0x002b
#define CMD_SET_FLY_X								0x002c
#define CMD_SET_FLY_Y								0x002d
#define CMD_SET_FLY_DELAY							0x002e
#define CMD_SET_SIMULATE_ENCODER					0x002f
#define CMD_REBOOT									0x0030
#define CMD_JOGSTART								0x0031
#define CMD_JOGEND									0x0032
#define CMD_SET_MOTION_BACKSPACE					0x0033
#define CMD_SET_MOTION_HOME_REAL					0x0034
#define CMD_RESET_ENCODER							0x0035
#define CMD_R05_CONFIG								0x0036
#define CMD_R05_LEVEL								0x0037
#define CMD_WRITE_IO_PORT_EX						0x0038
#define CMD_DSP_START								0x0039
#define CMD_SET_PROGRAM_READY						0x003a
#define CMD_SET_MARKING_READY						0x003b
#define CMD_SET_MARKING_END 						0x003c
#define CMD_SET_MARKING_END_TIME					0x003d
#define CMD_RESET_PULSE_COUNT						0x003e
#define CMD_SET_LABEL								0x003f
#define CMD_GOTO_XYZ								0x0040
#define CMD_SET_ENCODER_MODE						0x0041
#define CMD_SET_ENCODER_COMP						0x0042
#define CMD_SET_MOTION_XY							0x0043
#define CMD_SET_CARD_CONTROL_STATUS					0x0044
#define CMD_SET_FLY_PREVIEW_MODE					0x0045
#define CMD_SET_IO_STARTSTOP						0x0046
#define CMD_EXEC_LIST								0x0047
#define CMD_STOP_LIST								0x0048
#define CMD_STOP_EXEC								0x0049
#define CMD_START_LIST								0x004a
//#define CMD_STOP_EXECUTION							0x004b
#define CMD_MOTION_ARC_AXIS							0x004c
#define CMD_MOTION_ARC_LON_STEP						0x004d
#define CMD_MOTION_ARC_CLR							0x004e
#define CMD_MOTION_ARC_START						0x004f
#define CMD_MOTION_ARC_ADD							0x0050
#define CMD_ENCODER_MOTION							0x0051
#define CMD_SET_STARTSTOP_FILTER					0x0052
#define CMD_SET_IO_FILTER_LAG						0x0053
#define CMD_SCAN_XY_ENABLE							0x0054
#define CMD_LOAD_CORRECTION_FILE					0x0055
#define CMD_LOAD_CORRECTION_FILE_3D					0x0056
#define CMD_LOAD_VARPOLYDELAY						0x0057
#define CMD_WRITE_MEM								0x0058
#define CMD_SET_LENS_TRANSFORM						0x0059
#define CMD_SET_XY_SWAP								0x005a
#define CMD_SET_XYZ_REVERSE							0x005b
#define CMD_SET_MOTION_INDEX_DELAY					0x005c
#define CMD_SET_MOTION_SOFT_LIMIT					0x005d
#define CMD_SET_ENCODER_PARAM						0x005e
#define CMD_ENABLE_3D_MULTI_COR						0x005f
//Bill_20160912 支援聚焦軸Z補償
#define CMD_SET_Z_COMP_TABLE						0x0060
//Bill_20161117 Add Encoder Pos Trigger Mode
#define CMD_SET_ENCODER_POS_TRIGGER					0x0061
//Bill_20161215 新增龍門軸設定
#define CMD_SET_MOTION_GANTRY						0x0062
#define CMD_SET_MOTION_IO							0x0063
#define CMD_SET_MOTION_POS_ERR						0x0064
#define CMD_MOTION_SERVO_ON							0x0065
//Bill_20161223 修改支援Scanner PSO
#define CMD_SET_SCAN_PSO							0x0066
//Bill_20160206 新增極耳邊距設定
#define CMD_SET_ETAB_INTERVAL						0x0067
//Bill_20170221 新增PSO濾波設定
#define CMD_SET_PSO_FILTER							0x0068
//Bill_20170223 調整PSO控制模式 (新增額外PWM Freq)
#define CMD_SET_PSO_PWM2							0x0069
#define CMD_SET_PSO_LASER_MODE						0x006a
//Bill_20170314 Add ETab io commands
#define CMD_SET_ETAB_TRG_IN							0x006b
#define CMD_SET_ETAB_OUT							0x006c
//Bill_20170323 修改Mark on Fly Encoder Mode參考模式
#define CMD_SET_ENCODER_FLY_REF_MODE				0x006d
//bill_20170511 支援四軸同動修改
#define CMD_SET_MOTION_SCAN_PARAMS					0x006e
//Bill_20170616 新增雷射虛線設定
#define CMD_SET_DOTLINE_PARAMS	  		  			0x006f
//Bill_20170630 新增3D Z Scale設定
#define CMD_SET_3D_Z_SCALE							0x0070
//Bill_20170912 新增即時指令直接設定PSO參數
#define CMD_SET_SCAN_PSO_DIRECT						0x0071
//Bill_20170925 四軸同動整合調整修改
#define CMD_MOTION_ARC_CLR_EX						0x0072
#define CMD_MOTION_ARC_START_EX						0x0073
#define CMD_MOTION_ARC_ADD_EX						0x0074
//Bill_20171218 支援極耳紅光預覽
#define CMD_SET_ETAB_PREVIEW_MODE					0x0075
//Bill_20180103 支援極耳不良品信號保持距離設定
#define CMD_SET_NG_KEEP_DISTANCE					0x0076
//Bill_20180108 錦帛方: 新增極耳功率變更控制
#define CMD_SET_IPG_POWER_SETTING					0x0077
//Bill_20180119 新增不良品不作用距離設定 (雙卡同步處理)
#define CMD_SET_NG_DISABLE_DISTANCE					0x0078
//Bill_20180227 支援Disable STOP功能
#define CMD_DISABLE_STOP							0x0079
//Bill_20180504 支援功率校正表設定
#define CMD_LOAD_POWER_COR_FILE						0x007a
#define CMD_SET_POWER_COR_PARAMS					0x007b
#define CMD_SET_DIGITAL_POWER_OUT					0x007c
#define CMD_SET_POWER								0x007d
//Bill_20180517 新增功率控制最大最小值限制
#define CMD_SET_MAXMIN_POWER_VALUE					0x007e
//Bill_20180605 錦帛方極耳: 新增速度功率控制設定
#define CMD_SET_ETAB_SPEED_POWER_CONTROL			0x007f
//Bill_20180628 支援虛線振鏡位置等待開關雷射模式設定 & 振鏡PSO功能
#define CMD_SET_SCAN_FEEDBACK_MODE					0x0080
#define CMD_SET_PSO_MODE							0x0081
//Bill_20180704 新增前置校正表處理
#define CMD_SET_EXT_COR_TABLE						0x0082
//Bill_20180910 新增極耳流水線方向設定
#define CMD_SET_NG_RESET_POINT						0x0083
#define CMD_SET_ETAB_ENCODER_DIRECTION				0x0084
//Bill_20181113 支援四軸同動Encoder補償設定
#define CMD_SET_MOTION_SCAN_FB_COMP					0x0085
#define CMD_SET_MOTION_SCAN_FB_COMP_DATA			0x0086
//Bill_20190312 修改支援特殊條件NG Jump處理
#define CMD_SET_NG_JUMP_CONDITION					0x0087
//Bill_20190417 支援自訂路徑Wobble設定
#define CMD_SET_MULTI_PATH_WOBBEL_PARAM				0x0088
#define CMD_LOAD_MULTI_PATH_WOBBEL_SEGMENT			0x0089
//Bill_20190530 新增AMOF Motion補償位置預測設定
#define CMD_SET_AMOF_PREDICTION_TIME				0x008a
//Bill_20190604 支援極耳NG2信號處理
#define CMD_SET_NG2_PARAMS							0x008b
//Bill_20190627 極耳: 新增速度頻率控制設定
#define CMD_SET_ETAB_SPEED_FREQ_CONTROL				0x008c
//Bill_20190717 新增雷射信號延遲設定(Scanlab execiiSCAN控制需求)
#define CMD_SET_LASER_SIGNAL_LAG					0x008d
//Bill_20190905 新增ENCODER JUMP參數更新命令
#define CMD_SET_ENCODER_JUMP_UPDATE					0x008e
//Bill_20191004 新增Scanner 2固定Wobble輸出模式
#define CMD_SET_SCAN_CTRL_MODE						0x008f
#define CMD_SET_WOBBEL_EX							0x0090
//Bill_20191028 支援IP IPG YLPM設定
#define CMD_SET_IPG_PD_CFG							0x0091
#define CMD_SET_IPG_PD_CMD							0x0092
//Bill_20191031 新增雷射輸出延後設定
#define CMD_SET_LASER_SHIFT							0x0093
//Bill_20191113 支援雷射內頻對齊功能設定
#define CMD_SET_INT_FREQ_ALIGN_PARAMS				0x0094
//Bill_20191120 新增即時指令GOTO速度設定
#define CMD_SET_GOTO_SPEED							0x0095
//Bill_20191121 支援IPG Power功率設定(IP)
#define CMD_ENABLE_IPG_POWER_MODE					0x0096
#define CMD_SET_IPG_DIGITAL_POWER					0x0097
//Bill_20191217 修改切割路徑支援振鏡控制模式
#define CMD_SET_CUT_SCAN_RATE						0x0098
//Bill_20191219 支援手搖輪設定
#define CMD_SET_ENCODER_MOTION						0x0099
//Bill_20200108 支援馬達模型設定
#define CMD_SET_MOTION_MODEL_ENABLE					0x009a
#define CMD_SET_MOTION_MODEL_PARAMS					0x009b
//Bill_20200120 修改Scanner 2固定Wobble輸出模式支援螺旋
#define CMD_SET_SCAN_CTRL_LASERON_KEEP				0x009c
//Bill_20200221 支援軸控速度功率補償設定
#define CMD_SET_MOTION_SPEED_POWER_PARAMS			0x009d
//Bill_20200225 新增LaserSignalOn Delay
#define CMD_SET_LASER_SIGNAL_DELAY					0x009e
//Bill_20200302 支援Motion Route S形加速設定
#define CMD_SET_MOTION_ROUTE_PARAMS					0x009f
//Bill_20200416 支援IP軸控PSO設定
#define CMD_SET_MOTION_PSO_MODE						0x00a0
//Bill_20200505 支援圓盤旋轉軸飛雕設定
#define CMD_SET_ROT_CENTER							0x00a1
//Bill_20200824 新增Encoder設定函式
#define CMD_SET_MOTION_ENCODER						0x00a2
//Bill_20200904 支援ECM-SK EtherCAT卡控制
#define CMD_SET_ECM_SK_CONFIG						0x00a3
#define CMD_SET_ECM_SK_DATA							0x00a4
#define CMD_SET_ECM_SK_DATA_EX						0x00a5
//Bill_20200927 支援軸控干涉表補償
#define CMD_SET_MOTION_COMP_PARAMS					0x00a6
#define CMD_SET_MOTION_COMP_DATA					0x00a7
//Bill_20201209 新增切割路徑軸設定
#define CMD_SET_MOTION_ROUTE_AXIS_MAP				0x00a8
//Bill_20201222 極耳新增Wait Encoder Trigger設定
#define CMD_SET_WAIT_ENCODER_TRG_PARAM				0x00a9
//Bill_20210112 新增執行中List Jump命令
#define CMD_SET_EXEC_LIST_JUMP_POS					0x00aa
//Bill_20210114 新增Laser Shift Delay (振鏡模型使用)
#define CMD_SET_LASER_SHIFT_TIME					0x00ab
//Bill_20210201 修改AMOF軸可設定
#define CMD_SET_AMOF_AXIS_MAP						0x00ac
//Bill_20210217 支援固定Acc振鏡模型設定
#define CMD_SET_SCAN_MODEL_MODE						0x00ad
#define CMD_SET_SCAN_ACC_MODEL_PARAMS				0x00ae
//Bill_20210224 支援RingList自動化流程修改
#define CMD_RING_BUFFER_COUNT_ADD					0x00af
//Bill_20210330 支援PWM Burst模式設定
#define CMD_SET_PWM_BURST_PARAMS					0x00b0

//List Command  (0x8000)
#define CMD_LIST_JUMP_TO							0x8000
#define CMD_LIST_MARK_TO							0x8001
#define CMD_LIST_JUMP_SPEED							0x8002
#define CMD_LIST_MARK_SPEED							0x8003
#define CMD_LIST_SET_LASER_DELAY					0x8004
#define CMD_LIST_SET_SCANNER_DELAY					0x8005
#define CMD_LIST_SET_LASER_TIMING					0x8006
#define CMD_LIST_LASER_ON							0x8007
#define CMD_LIST_LASER_SIGNAL_ON					0x8008
#define CMD_LIST_SET_LASER_FPK						0x8009
#define CMD_LIST_SET_STANDBY						0x800a
#define CMD_LIST_ARC_ABS							0x800b
#define CMD_LIST_ARC_REL							0x800c
#define CMD_LIST_CLEAR_IO_COND_LIST					0x800d
#define CMD_LIST_JUMP_ABS							0x800e
#define CMD_LIST_JUMP_REL							0x800f
//#define CMD_LIST_LASER_ON_LIST						0x8010
#define CMD_LIST_LIST_CALL							0x8011
#define CMD_LIST_CALL_COND							0x8012
#define CMD_LIST_JUMP_COND							0x8013
#define CMD_LIST_LIST_NOP							0x8014
#define CMD_LIST_LIST_RETURN						0x8015
#define CMD_LIST_LONG_DELAY							0x8016
#define CMD_LIST_MARK_ABS							0x8017
#define CMD_LIST_MARK_REL							0x8018
#define CMD_LIST_SAVE_AND_RESTART_TIMER				0x8019
#define CMD_LIST_SET_CONTROL_MODE_LIST				0x801a
#define CMD_LIST_SET_END_OF_LIST					0x801b
#define CMD_LIST_SET_EXTSTARTPOS_LIST				0x801c
#define CMD_LIST_SET_IO_COND_LIST					0x801d
//#define CMD_LIST_LASER_TIMING						0x801e
#define CMD_LIST_SET_LIST_JUMP						0x801f
#define CMD_LIST_SET_MATRIX_LIST					0x8020
#define CMD_LIST_SET_OFFSET_LIST					0x8021
#define CMD_LIST_SET_PIXEL							0x8022
#define CMD_LIST_SET_PIXEL_LINE						0x8023
#define CMD_LIST_SET_SCANNER_DELAYS					0x8024
//#define CMD_LIST_LASER_SIGNAL_ON_LIST				0x8025
#define CMD_LIST_SET_TRIGGER						0x8026
#define CMD_LIST_SET_WAIT							0x8027
#define CMD_LIST_SET_WOBBEL							0x8028
#define CMD_LIST_TIMED_JUMP_ABS						0x8029
#define CMD_LIST_TIMED_JUMP_REL						0x802a
#define CMD_LIST_TIMED_MARK_ABS						0x802b
#define CMD_LIST_TIMED_MARK_REL						0x802c
#define CMD_LIST_WRITE_8BIT_PORT_LIST				0x802d
#define CMD_LIST_WRITE_DA_X_LIST					0x802e
#define CMD_LIST_WRITE_IO_PORT_LIST					0x802f
#define CMD_LIST_Z_OUT_LIST							0x8030
//#define CMD_SET_START_LIST							0x8031
//#define CMD_LIST_LASER_SIGNAL_OFF_LIST				0x8032
#define CMD_LIST_MOTION_MOVETO						0x8033
#define CMD_LIST_SET_MOTION_XYTABLE					0x8034
#define CMD_LIST_MARK_SPOT							0x8035
#define CMD_LIST_WRITE_IO_PORT_LIST_EX				0x8036
#define CMD_SET_MARKING_READY_LIST					0x8037
#define CMD_SET_MOTION_HOME_LIST					0x8038
#define CMD_SET_XYTABLE_HOME_LIST					0x8039
#define CMD_SET_LABEL_LIST							0x803a
#define CMD_SET_COND_TIMEOUT						0x803b
#define CMD_SET_FLY_X_LIST							0x803c
#define CMD_SET_FLY_Y_LIST							0x803d
#define CMD_SET_FLY_DELAY_LIST						0x803e
#define CMD_SET_SIMULATE_ENCODER_LIST	   			0x803f
#define CMD_LIST_JUMP_3D_ABS						0x8040
#define CMD_LIST_JUMP_3D_REL						0x8041
#define CMD_LIST_MARK_3D_ABS						0x8042
#define CMD_LIST_MARK_3D_REL						0x8043
#define CMD_LIST_TIMED_JUMP_3D_ABS					0x8044
#define CMD_LIST_TIMED_JUMP_3D_REL					0x8045
#define CMD_LIST_TIMED_MARK_3D_ABS					0x8046
#define CMD_LIST_TIMED_MARK_3D_REL					0x8047
#define CMD_LIST_RESET_PULSE_COUNT					0x8048
#define CMD_LIST_SET_MOTION_XY						0x8049
#define CMD_LIST_SET_LAYER_ENCODER_CNT				0x804a
#define CMD_LIST_WAIT_ENCODER_CNT					0x804b
#define CMD_LIST_JOGSTART							0x804c
#define CMD_LIST_JOGEND								0x804d
#define CMD_LIST_MARK_POLY							0x804e
#define CMD_LIST_DOT_MARK_ABS						0x804f
#define CMD_LIST_DOT_MARK_3D_ABS					0x8050
#define CMD_LIST_SET_PWM_VARPERIOD					0x8051
#define CMD_LIST_SET_WOBBEL_2						0x8052
#define CMD_LIST_SET_SKYWRITING_MODE				0x8053
#define CMD_LIST_SCAN_XY_ENABLE						0x8054
#define CMD_LIST_SYN_CONDITION						0x8055
#define CMD_LIST_MOTION_ARC_ADD_XY					0x8056
#define CMD_LIST_MOTION_ARC_ADD_RZ					0x8057
#define CMD_LIST_MOTION_ARC_ADD_TIME				0x8058
#define CMD_LIST_MOTION_ARC_LON_STEP				0x8059
#define CMD_LIST_MOTION_ARC_START					0x805a
#define CMD_LIST_TIMED_ARC_ABS						0x805b
#define CMD_LIST_TIMED_ARC_REL						0x805c
#define CMD_LIST_ARC_3D_ABS							0x805d
#define CMD_LIST_ARC_3D_REL							0x805e
#define CMD_LIST_TIMED_ARC_3D_ABS					0x805f
#define CMD_LIST_TIMED_ARC_3D_REL					0x8060
//Bill_20160922 新增List多軸控制 (可指定)
#define CMD_LIST_MOTION_MOVETO_MULTI				0x8061
//Bill_20161223 修改支援Scanner PSO
#define CMD_LIST_SET_SCAN_PSO						0x8062
//Bill_20170104 新增Encoder Move特殊處理(錦帛方電池極耳切割案)
#define CMD_LIST_WAIT_ENCODER						0x8063
#define CMD_LIST_ENCODER_JUMP						0x8064
#define CMD_LIST_SET_ENCODER_FLY					0x8065
//Bill_20160206 新增指定Loop
#define CMD_LIST_SET_LOOP_CNT						0x8066
#define CMD_LIST_SET_LOOP_JUMP						0x8067
//Bill_20170323 修改支援Motion直接控制命令
#define CMD_LIST_MOTION_DIRECT_START				0x8068
#define CMD_LIST_MOTION_DIRECT_MOVE					0x8069
//bill_20170511 支援四軸同動修改
#define CMD_LIST_MOTION_SCAN_ENABLE					0x806a
#define CMD_LIST_SET_WAIT_MOTION_POS				0x806b
#define CMD_LIST_WAIT_MOTION_POS					0x806c
//Bill_20170616 新增雷射虛線設定
#define CMD_LIST_SET_DOTLINE_PARAMS	  		  		0x806d
#define CMD_LIST_SELECT_LIST						0x806e	//Bill_20170808 調整為多List架構
//Bill_20170830 新增虛線開關時間設定
#define CMD_LIST_SET_DOTLINE_ONOFF					0x806f
//Bill_20170912 新增調整弧相關命令
#define CMD_LIST_SET_ELLIPSE						0x8070
#define CMD_LIST_ELLIPSE_ABS						0x8071
#define CMD_LIST_ELLIPSE_REL						0x8072
//Bill_20170922 錦帛方: 新增NG信號設定處理
#define CMD_LIST_SET_NG_RESET_POINT					0x8073
//Bill_20170925 四軸同動整合調整修改
#define CMD_LIST_MOTION_ARC_ADD_X_EX				0x8074
#define CMD_LIST_MOTION_ARC_ADD_Y_EX				0x8075
#define CMD_LIST_MOTION_ARC_ADD_TIME_EX				0x8076
#define CMD_LIST_MOTION_ARC_START_EX				0x8077
//Bill_20171019 四軸同動新增時間紀錄命令
#define CMD_LIST_RECORD_MARK_TIME					0x8078
//Bill_20180504 支援功率校正表設定
#define CMD_LIST_SET_POWER							0x8079
//Bill_20180625 支援NG信號切線控制處理
#define CMD_LIST_NG_LIST_JUMP						0x8080
//Bill_20180628 支援虛線振鏡位置等待開關雷射模式設定 & 振鏡PSO功能
#define CMD_LIST_SET_SCAN_FEEDBACK_MODE				0x8081
#define CMD_LIST_SET_PSO_MODE						0x8082
//Bill_20190109 新增AMOF同步命令
#define CMD_LIST_AMOF_SYNC							0x8083
//Bill_20190417 支援進階Wobble設定
#define CMD_LIST_SET_ADV_WOBBEL_MODE				0x8084
#define CMD_LIST_SET_ADV_WOBBEL_WAVE				0x8085
//Bill_20190905 新增ENCODER JUMP參數更新命令
#define CMD_LIST_ENCODER_JUMP_UPDATE				0x8086
//Bill_20191004 新增Scanner 2固定Wobble輸出模式
#define CMD_LIST_SET_WOBBEL_EX						0x8087
//Bill_20191028 支援IP IPG YLPM設定
#define CMD_LIST_SET_IPG_PD_CMD						0x8088
#define CMD_LIST_WAIT_IPG_PD						0x8089
//Bill_20191121 支援IPG Power功率設定(IP)
#define CMD_LIST_SET_IPG_DIGITAL_POWER				0x808a
//Bill_20191211 Image Pixel支援3D位置設定
#define CMD_LIST_SET_PIXEL_EX						0x808b
//Bill_20191217 修改切割路徑支援振鏡控制模式
#define CMD_LIST_SET_CUT_SCAN_PARAMS				0x808c
//Bill_20200120 修改Scanner 2固定Wobble輸出模式支援螺旋
#define CMD_LIST_SET_WOBBEL_EX2						0x808d
//Bill_20200203 支援螺旋命令設定
#define CMD_LIST_SET_SPIRAL_PARAM1					0x808e
#define CMD_LIST_SET_SPIRAL_PARAM2					0x808f
#define CMD_LIST_SPIRAL_ABS							0x8090
#define CMD_LIST_SPIRAL_REL							0x8091
//Bill_20200221 支援軸控速度功率補償設定
#define CMD_LIST_SET_POWER_COR_PARAMS				0x8092
//Bill_20200225 新增LaserSignalOn Delay
#define CMD_LIST_SET_LASER_SIGNAL_DELAY				0x8093
//Bill_20200319 旋轉軸向量路徑變更為AMOF路徑規畫路徑修改
#define CMD_LIST_MOTION_ARC_ADD_R_EX    			0x8094
#define CMD_LIST_MOTION_ARC_ADD_Z_EX    			0x8095
#define CMD_LIST_MOTION_ARC_ADD_TIME2_EX    		0x8096
//Bill_20200416 修改軸控速度功率補償設定(支援不同軸單位)
#define CMD_LIST_SET_MOTION_SPEED_POWER_PARAMS		0x8097
#define CMD_LIST_SET_MOTION_SPEED_UNIT_RATE			0x8098
//Bill_20200505 支援圓盤旋轉軸飛雕設定
#define CMD_LIST_SET_ROT_CENTER						0x8099
#define CMD_LIST_SET_FLY_ROT						0x809a
//Bill_20200601 支援內頻狀態檢查 & List命令設定
#define CMD_LIST_SET_INT_FREQ_ALIGN_PARAMS			0x809b
//Bill_20201222 極耳新增Wait Encoder Trigger設定
#define CMD_LIST_WAIT_ENCODER_TRIGGER				0x809c
//Bill_20210112 新增執行中List Jump命令
#define CMD_LIST_SET_EXEC_LIST_JUMP					0x809d
//Bill_20210114 支援曲線虛線模式修改
#define CMD_LIST_CURVE_DOTLINE_MODE					0x809e
//Bill_20210122 支援曲線虛線多線段輸出設定
#define CMD_LIST_DASH_LINE_PARAMS					0x809f
#define CMD_LIST_DASH_LINE							0x80a0
//Bill_20210224 支援RingList自動化流程修改
#define CMD_LIST_SET_EXT_START_POS					0x80a1
//Bill_20210330 支援PWM Burst模式設定
#define CMD_LIST_SET_PWM_BURST_PARAMS				0x80a2


//Error Code  (0x0400)
#define CMD_ERR_CODE								0x2000
#define CMD_ERR_SIZENOTMATCH						0x2001
#define CMD_ERR_ADDLISTBUFFAIL						0x2002
#define CMD_ERR_CTRLCMDUNKNOWN						0x2003
#define CMD_ERR_WRONGPARAMETER						0x2004

#define ERR_CODE_1	1	//RS232 Check Sum Error
#define ERR_CODE_2	2	//RS232 Reading Time out
#define ERR_CODE_3	3	//RS232 End Code Error

#define GPD_TYPE 40000

//Bill_20090303_定義工作週期
#define CTRL_TIME	80	
#define MIN_STEP	20	

// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
#define IOCTL_GPD_READ_PORT_UCHAR \
    CTL_CODE( GPD_TYPE, 0x900, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_PORT_USHORT \
    CTL_CODE( GPD_TYPE, 0x901, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_PORT_ULONG \
    CTL_CODE( GPD_TYPE, 0x902, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_WRITE_PORT_UCHAR \
    CTL_CODE(GPD_TYPE,  0x910, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_USHORT \
    CTL_CODE(GPD_TYPE,  0x911, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_ULONG \
    CTL_CODE(GPD_TYPE,  0x912, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_GET_MAPPED_MEMORY_BASES \
    CTL_CODE( GPD_TYPE, 0x913, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_GET_PORT_BASES \
    CTL_CODE( GPD_TYPE, 0x914, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_GET_MAPPED_MEMORY_COUNT \
    CTL_CODE( GPD_TYPE, 0x915, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_GET_PORT_COUNT \
    CTL_CODE( GPD_TYPE, 0x916, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_MEMORY_UCHAR \
    CTL_CODE( GPD_TYPE, 0x917, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_MEMORY_USHORT \
    CTL_CODE( GPD_TYPE, 0x918, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_MEMORY_ULONG \
    CTL_CODE( GPD_TYPE, 0x919, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_WRITE_MEMORY_UCHAR \
    CTL_CODE(GPD_TYPE,  0x91A, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_MEMORY_USHORT \
    CTL_CODE(GPD_TYPE,  0x91B, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_MEMORY_ULONG \
    CTL_CODE(GPD_TYPE,  0x91C, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_GET_NUM_OF_MAPPED_MEMORY_BASES \
    CTL_CODE( GPD_TYPE, 0x91D, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_GET_NUM_OF_PORT_BASES \
    CTL_CODE( GPD_TYPE, 0x91E, METHOD_BUFFERED, FILE_READ_ACCESS )
//Bill_20100709 add block trans
#define IOCTL_GPD_TRANSMITBLOCK \
    CTL_CODE( GPD_TYPE, 0x91F, METHOD_BUFFERED, FILE_READ_ACCESS )


//SHA204 Define
//Error Code
#define SHA204_SUCCESS              0x00 //!< Function succeeded.
#define SHA204_NO_COMMAND           0x01 //!< Default return value
#define SHA204_BUFFER_TO_SMALL      0x02 //!< Default return value
#define SHA204_PARSE_ERROR          0xD2 //!< response status byte indicates parsing error
#define SHA204_CMD_FAIL             0xD3 //!< response status byte indicates command execution error
#define SHA204_STATUS_CRC           0xD4 //!< response status byte indicates CRC error
#define SHA204_STATUS_UNKNOWN       0xD5 //!< response status byte is unknown
#define SHA204_FUNC_FAIL            0xE0 //!< Function could not execute due to incorrect condition / state.
#define SHA204_GEN_FAIL             0xE1 //!< unspecified error
#define SHA204_BAD_PARAM            0xE2 //!< bad argument (out of range, null pointer, etc.)
#define SHA204_INVALID_ID           0xE3 //!< invalid device id, id not set
#define SHA204_INVALID_SIZE         0xE4 //!< Count value is out of range or greater than buffer size.
#define SHA204_BAD_CRC              0xE5 //!< incorrect CRC received
#define SHA204_RX_FAIL              0xE6 //!< Timed out while waiting for response. Number of bytes received is > 0.
#define SHA204_RX_NO_RESPONSE       0xE7 //!< Not an error while the Command layer is polling for a command response.
#define SHA204_RESYNC_WITH_WAKEUP   0xE8 //!< re-synchronization succeeded, but only after generating a Wake-up

#define SHA204_COMM_FAIL            0xF0 //!< Communication with device failed. Same as in hardware dependent modules.
#define SHA204_TIMEOUT              0xF1 //!< Timed out while waiting for response. Number of bytes received is 0.

#define SHA204_NO_SUPPORT			0xFF
//Const
#define SHA204_RSP_MAX				35
#define SHA204_RSP_ERRCODE_SIZE		5