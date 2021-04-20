#ifndef _EMC6_DEF_
#define _EMC6_DEF_

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

#ifndef CALLBACK
#define CALLBACK    __stdcall
#endif

#define POLYDELAY_FILE_SIZE 201

#include <Windows.h>

//===============================================
//原始定義在GalvoDriver.h,有變動必須同時修改
//===============================================
//motion parameter structure 
typedef	struct
{
	long			m_lAccTime;				// unit[ms]
	long			m_lRotarySpeed;			// motion Speed [pulse/sec]
	long			m_lPulseCount;			// resolution : [pulse counts/degree * 1000] or  x-y-z : [pulse/mm * 1000]
	long			m_bGoTo0_MarkEnd;		// mark end motion to zero. (reserve, it is not used) 
	long			m_lMotorCount;			// pulse counts (reserve, it is not used)
	long			m_bAH_InPos;			// inpos snesor signal sensibility level (1/0)
	long			m_bAH_Home;				// mome snesor signal sensibility level (1/0)
	long			m_lIniSpeed;			// Init Speed [pulse/sec]
	long			m_bReverse;				// motion reverse
	long			m_bAH_Limit;			// limit snesor signal sensibility level (1/0)
	long			m_lHomeSpeed;			// enter Home Speed [pulse/sec]
	long			m_lHomeBackSpeed;		// leave Home Speed [pulse/sec]
	long			m_bEnable;				// enable (reserve, it is not used)
	long			m_lINPos_TimeOut;		// inpos sensor Timer out [ms]
	long			m_lINPos_Delay;			// Motion inposition dleay [ms]
} ROTARY_PARAM;


typedef struct
{
	char	szHeader[32];	//Elogic COR2 file format
	short	lVersion;		//version = lVersion/100
	short	sGridCnt;		//Grid Count 65x65=65
	double	dDataScale;		//資料縮放比例
	BYTE	byUnused[16];	//保留		
}COR2_HEAD;

//Bill_20160912 支援聚焦軸Z補償
typedef struct
{
	long lZPos;
	long lZCompPos;
	double dScaleXp;
	double dScaleXn;
	double dScaleYp;
	double dScaleYn;
}Z_COMP_PARAMS;
//Bill_20160912 支援聚焦軸Z補償
//===============================================

//////////////////////////////////////////////////
// Initialization And Field Corection
//////////////////////////////////////////////////
EXPORT	long	CALLBACK	load_program_file (const char* FileName);
EXPORT	long	CALLBACK	load_correction_file (const char* FileName, long cor_table, double kx, double ky, double phi, double x_offset, double y_offset);
EXPORT	long	CALLBACK	load_correction_file_3d (const char* FileName,long cor_table,long lZOffset);
EXPORT	void	CALLBACK	select_cor_table (unsigned long head_a, unsigned long head_b);
EXPORT	long	CALLBACK	load_varpolydelay (const char* STBFileName, unsigned long TableNo);
EXPORT	void	CALLBACK	dsp_start (void);

//////////////////////////////////////////////////
// Laser Mode And Parameters
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_laser_mode (unsigned long mode);
EXPORT	void	CALLBACK	set_firstpulse_killer (long fpk,unsigned long lead_time);
EXPORT	void	CALLBACK	set_softstart_level (unsigned long index, unsigned long level);
EXPORT	void	CALLBACK	set_softstart_mode (unsigned long mode, unsigned long number, unsigned long restartdelay);
EXPORT	void	CALLBACK	set_standby(unsigned long half_period, unsigned long pulse_width);
EXPORT void		CALLBACK	set_softstart_pwm(long sNum,long lApendDelay);
//////////////////////////////////////////////////
// Scanner Delay Mode
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_delay_mode (unsigned long vapoly, unsigned long directmove3d, unsigned long edgelevel, unsigned long MinJumpDelay, unsigned long JumpLengthLimit);
				
//////////////////////////////////////////////////
// Coordinate Transformations
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_matrix (double m11, double m12, double m21, double m22);
EXPORT	void	CALLBACK	set_offset (long x_offset, long y_offset);
EXPORT	void	CALLBACK	set_lens_transform (long Head,double m11, double m12, double m21, double m22,long x_offset, long y_offset);
EXPORT	void	CALLBACK	set_xy_swap(long Head,long Swap);
EXPORT	void	CALLBACK	set_xyz_reverse(long Head,long x,long y,long z);

//////////////////////////////////////////////////
// Status Monitoring and Diagnostics
//////////////////////////////////////////////////
EXPORT	unsigned long	CALLBACK	get_head_status (unsigned long head);
EXPORT	long	CALLBACK	get_value (long signal,long *pData);
EXPORT	void	CALLBACK	get_waveform (unsigned long channel, unsigned long stop, signed long *memptr);
EXPORT	void	CALLBACK	measurement_status (unsigned long *busy, unsigned long *position);
				
//////////////////////////////////////////////////
// I/O commands
//////////////////////////////////////////////////
EXPORT	unsigned long CALLBACK	get_io_status (void);
EXPORT	unsigned long CALLBACK	read_io_port (void);
EXPORT	void	CALLBACK	write_8bit_port (unsigned long Bit,unsigned long bOn);
EXPORT	void	CALLBACK	write_da_x (unsigned long x, unsigned long value);
EXPORT	void	CALLBACK	write_da_1 (unsigned long value);
EXPORT	void	CALLBACK	write_da_2 (unsigned long value);
EXPORT	void	CALLBACK	write_io_port (unsigned long value);
EXPORT void		CALLBACK	set_io_startstop(long sStart,long sStop,long sStartStopAH);
				
//////////////////////////////////////////////////
// Externl Control Inputs
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_control_mode (unsigned long control_mode);
EXPORT	void	CALLBACK	select_list (unsigned long list);
EXPORT	void	CALLBACK	set_max_counts (long max_count);
EXPORT	long	CALLBACK	get_counts (void);
EXPORT	unsigned long	CALLBACK	get_startstop_info (void);

//////////////////////////////////////////////////
// List Handling And Status
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_start_list (unsigned long list_no);
EXPORT	void	CALLBACK	set_start_list_1 (void);
EXPORT	void	CALLBACK	set_start_list_2 (void);
EXPORT	void	CALLBACK	execute_list (unsigned long list_no);
EXPORT	void	CALLBACK	execute_list_1 (void);
EXPORT	void	CALLBACK	execute_list_2 (void);
EXPORT	void	CALLBACK	stop_execution (void);
EXPORT	void	CALLBACK	stop_list (void);
EXPORT	void	CALLBACK	restart_list (void);
EXPORT	void	CALLBACK	get_status (unsigned long *busy, unsigned long *position);
EXPORT	unsigned long	CALLBACK	read_status (void);

//////////////////////////////////////////////////
// Synchronization Of Processing
//////////////////////////////////////////////////
EXPORT	unsigned long	CALLBACK	get_wait_status (void);
EXPORT	void	CALLBACK	release_wait (void);

//////////////////////////////////////////////////
// Automatic List Handling
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	auto_change (long lList);
EXPORT	void	CALLBACK	auto_change_pos (unsigned long start);
EXPORT	void	CALLBACK	start_loop (long lListNum);
EXPORT	void	CALLBACK	quit_loop (void);

//////////////////////////////////////////////////
// Circular Queue Mode
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_list_mode (unsigned long mode);
EXPORT	unsigned long	CALLBACK	get_list_space (void);

//////////////////////////////////////////////////
// Structured Programming
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_input_pointer (unsigned long pointer);
EXPORT	unsigned long	CALLBACK	get_input_pointer (void);
EXPORT	void	CALLBACK	execute_at_pointer (unsigned long pointer);

//////////////////////////////////////////////////
// Direct Laser And Scan Head Control
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	disable_laser (void);
EXPORT	void	CALLBACK	enable_laser (void);
EXPORT	void	CALLBACK	laser_signal_on (void);
EXPORT	void	CALLBACK	laser_signal_off (void);
EXPORT	void	CALLBACK	goto_xy (long xpos, long ypos);
EXPORT	void	CALLBACK	get_xy_pos (long *xpos, long *ypos);
EXPORT	void	CALLBACK	z_out (long value);
EXPORT	void	CALLBACK	goto_xyz (long xpos, long ypos,long zpos);				
EXPORT	void	CALLBACK	get_xyz_pos_real (long *xpos, long *ypos, long *zpos);

//////////////////////////////////////////////////
// Using Multiple EMC6 Boards In One Computer
//////////////////////////////////////////////////
EXPORT	unsigned long	CALLBACK	emc6_count_cards (void);
EXPORT	long	CALLBACK	select_emc6 (unsigned long CardNo);

//////////////////////////////////////////////////
// Other Control Commands
//////////////////////////////////////////////////
EXPORT	unsigned long	CALLBACK	get_dll_version (void);
EXPORT	unsigned long	CALLBACK	get_hex_version (void);
EXPORT	unsigned long	CALLBACK	get_emc6_version (void);
EXPORT	unsigned long	CALLBACK	get_serial_number (void);
EXPORT	void	CALLBACK	home_position (long xhome, long yhome);
EXPORT	void	CALLBACK	get_hi_data (unsigned long *x1, unsigned long *x2, unsigned long *y1, unsigned long *y2);
EXPORT	double	CALLBACK	get_time (void);
EXPORT	void	CALLBACK	set_piso_control (unsigned long L1, unsigned long L2);
EXPORT	long	CALLBACK	auto_cal (unsigned long head, unsigned long command);
EXPORT	long CALLBACK	get_list_size (void);
EXPORT void		CALLBACK	set_card_control_status(long status);
EXPORT void CALLBACK r05_cfg(long lInterval, long sInit);
EXPORT void CALLBACK r05_level(long sIndex, long lValue);
EXPORT	void CALLBACK set_label (long vlue);
EXPORT long CALLBACK get_label (void);
EXPORT	void	CALLBACK	set_cond_timeout (long value);		
EXPORT long		CALLBACK	get_serial_code(void);
EXPORT void		CALLBACK	set_serial_code(long lSerialCode);

/////////////////////////////////////////////////////////////////////////////
// List Commands 
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////
// Vector Commands
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	jump_abs (long xval, long yval);
EXPORT	void	CALLBACK	jump_rel (long dx, long dy);
EXPORT	void	CALLBACK	mark_abs (long xval, long yval);
EXPORT	void	CALLBACK	mark_rel (long dx, long dy);

EXPORT	void	CALLBACK	timed_jump_abs (long xval, long yval, long step);
EXPORT	void	CALLBACK	timed_jump_rel (long xval, long yval, long step);
EXPORT	void	CALLBACK	timed_mark_abs (long xval, long yval, long step);
EXPORT	void	CALLBACK	timed_mark_rel (long xval, long yval, long step);

EXPORT	void	CALLBACK	jump_3d_abs (long xval, long yval,long zval);
EXPORT	void	CALLBACK	jump_3d_rel (long xval, long yval,long zval);
EXPORT	void	CALLBACK	mark_3d_abs (long xval, long yval,long zval);
EXPORT	void	CALLBACK	mark_3d_rel (long xval, long yval,long zval);

EXPORT	void	CALLBACK	timed_jump_3d_abs (long xval, long yval,long zval, long step);
EXPORT	void	CALLBACK	timed_jump_3d_rel (long xval, long yval,long zval, long step);
EXPORT	void	CALLBACK	timed_mark_3d_abs (long xval, long yval,long zval, long step);
EXPORT	void	CALLBACK	timed_mark_3d_rel (long xval, long yval,long zval, long step);

EXPORT	void	CALLBACK	arc_abs (long x, long y, double angle);
EXPORT	void	CALLBACK	arc_rel (long dx, long dy, double angle);
EXPORT	void	CALLBACK	timed_arc_abs (long x, long y, double angle,long step);
EXPORT	void	CALLBACK	timed_arc_rel (long dx, long dy, double angle,long step);

EXPORT void		CALLBACK	dot_mark_abs(long x,long y,long lPeriod,long lLength);
EXPORT void		CALLBACK	dot_mark_3d_abs(long x,long y,long z,long lPeriod,long lLength);
//////////////////////////////////////////////////
// List Handling
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_end_of_list (void);
EXPORT	void	CALLBACK	set_extstartpos_list (unsigned long position);
EXPORT	void	CALLBACK	set_control_mode_list (unsigned long control_mode);

//////////////////////////////////////////////////
// Synchronization of Processing
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_wait (unsigned long wait_word);
EXPORT	void	CALLBACK	set_trigger (unsigned long sampleperiod, unsigned long signal1, unsigned long signal2);

				
//////////////////////////////////////////////////
// Structured Programming
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_list_jump (unsigned long address);
EXPORT	void	CALLBACK	list_call (unsigned long address);
EXPORT	void	CALLBACK	list_return (void);
EXPORT	void	CALLBACK	list_nop (void);

//////////////////////////////////////////////////
// Setting The Laser Parameters
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_laser_timing (unsigned long half_period, unsigned long pulse_width1, unsigned long pulse_width2, unsigned long time_base);
EXPORT	void	CALLBACK	set_firstpulse_killer_list (long fpk,unsigned long lead_time);
EXPORT	void	CALLBACK	set_standby_list (unsigned long half_period, unsigned long pulse_width);
				
//////////////////////////////////////////////////
// Setting The Scanner Parameters
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_laser_delays (long laser_on_delay, long laser_off_delay);
EXPORT	void	CALLBACK	set_scanner_delays (long jump_delay, long mark_delay, long polygon_delay);
EXPORT	void	CALLBACK	set_jump_speed (double jump_speed);
EXPORT	void	CALLBACK	set_mark_speed (double mark_speed);
EXPORT	void	CALLBACK	set_laser_delays_ex (long laser_on_delay, long laser_off_delay);

//////////////////////////////////////////////////
// Coordinate Transformations
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_matrix_list (unsigned long i, unsigned long j, double m_ij);
EXPORT	void	CALLBACK	set_offset_list (long x_offset, long y_offset);

//////////////////////////////////////////////////
// Direct Laser And Scan Head Control
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	laser_on_list (long delay);
EXPORT	void	CALLBACK	laser_signal_on_list (void);
EXPORT	void	CALLBACK	laser_signal_off_list (void);
EXPORT	void	CALLBACK	z_out_list (long value);

//////////////////////////////////////////////////
// Scanning Raster Image (Bitmaps)
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	set_pixel_line (unsigned long pixel_mode, unsigned long pixel_period, double dx, double dy,unsigned long da_ch);
EXPORT	void	CALLBACK	set_pixel (unsigned long pulse_widht, unsigned long da_value);
EXPORT	void	CALLBACK	set_n_pixel (unsigned long pulse_width, unsigned long da_value,unsigned long num_cnt);

//////////////////////////////////////////////////
// I/O Commands
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	clear_io_cond_list (unsigned long select_io,unsigned long mask_1, unsigned long mask_0, unsigned long mask_clear);
EXPORT	void	CALLBACK	list_call_cond (unsigned long select_io,unsigned long mask_1, unsigned long mask_0, unsigned long address);
EXPORT	void	CALLBACK	list_jump_cond (unsigned long select_io,unsigned long mask_1, unsigned long mask_0, unsigned long address);
EXPORT	void	CALLBACK	set_io_cond_list (unsigned long select_io,unsigned long mask_1, unsigned long mask_0, unsigned long mask_set);
EXPORT	void	CALLBACK	write_8bit_port_list (unsigned long Bit,unsigned long bOn);
EXPORT	void	CALLBACK	write_da_x_list (unsigned long x, unsigned long value);
EXPORT	void	CALLBACK	write_da_1_list (unsigned long value);
EXPORT	void	CALLBACK	write_da_2_list (unsigned long value);
EXPORT	void	CALLBACK	write_io_port_list (unsigned long value);
EXPORT	void CALLBACK write_io_port_ex (unsigned long value);
EXPORT	unsigned long CALLBACK	get_io_status_ex (void);
EXPORT	void CALLBACK set_program_ready (unsigned long value);
EXPORT	void CALLBACK set_marking_ready (unsigned long value);
EXPORT	void CALLBACK set_marking_end (unsigned long value);
EXPORT	void CALLBACK set_marking_end_time (unsigned long value);
EXPORT	void	CALLBACK	set_marking_ready_list (unsigned long value);
				
//////////////////////////////////////////////////
// Other list Commands
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	long_delay (unsigned long delay);
EXPORT	void	CALLBACK	save_and_restart_timer (void);
EXPORT	void	CALLBACK	set_wobbel (unsigned long amplitude, double frequency);
EXPORT void		CALLBACK	set_softstart_pwm_list(long sNum,long lApendDelay);
EXPORT void		CALLBACK	set_wobbel_2(long sWidth,long sLength,long lFreq);
EXPORT void		CALLBACK	set_skywriting_mode(long lStartlag,long lEndlag);
///////////////////////////////////////////////////
//motion commands
//////////////////////////////////////////////////
EXPORT	void	CALLBACK	 set_xytable_parameters(unsigned char axis,ROTARY_PARAM* pParam);
EXPORT void CALLBACK set_xytable (long lR,long lX , long lY,long lZ);
EXPORT long CALLBACK set_xytable_home (unsigned char axis);
EXPORT void CALLBACK motion_moveto (unsigned char axis, long lPulse);
EXPORT void CALLBACK motion_moveto_real (unsigned char axis, long lPulse);
EXPORT long CALLBACK get_rotary_pulse_count(unsigned char axis);
EXPORT long CALLBACK get_motion_inpos(void);
EXPORT void CALLBACK list_set_xytable (long lR,long lX , long lY,long lZ);
EXPORT void CALLBACK list_motion_moveto (unsigned char axis, long lPulse);
EXPORT void CALLBACK motion_jog_start (int iAxis, long dSpeed, long dAccTime, int iDir);
EXPORT void CALLBACK motion_jog_end (int iAxis);
EXPORT void CALLBACK motion_get_encoder (long *pusEList);
EXPORT long CALLBACK set_home_real (long axis);
EXPORT void CALLBACK motion_reset_encoder (long axis);
EXPORT void CALLBACK mark_spot(long xval, long yval , long delay);
EXPORT void CALLBACK motion_home_list (long axis);
EXPORT void CALLBACK xytable_home_list (long sel);
EXPORT void CALLBACK reset_motion_pulse_count (long axis,long pulse);
EXPORT	long CALLBACK get_motor_sensor(void);
EXPORT	long CALLBACK get_motor_status(void);
EXPORT	void	CALLBACK set_home_direction(long axis,long dir);
EXPORT	void	CALLBACK	set_motion_backspace (long axis, long pulse);
EXPORT void		CALLBACK	list_set_motion_pos(long sxis,long pulse);
EXPORT void		CALLBACK	list_motion_xy(long xval,long yval);
EXPORT void		CALLBACK	set_motion_xy(long xval,long yval);
EXPORT void CALLBACK motion_jog_start_list (int iAxis, long dSpeed, long dAccTime, int iDir);
EXPORT void CALLBACK motion_jog_end_list (int iAxis);
EXPORT	void	CALLBACK set_home_mode(long axis,long sMode);
EXPORT	BOOL	CALLBACK set_EncoderMotion_mode(long sEnable,long sEncoder,long Index,long lTbValue,long lScale,long lLimitAcc);
EXPORT void CALLBACK set_xytable_real (long lR,long lX , long lY,long lZ);
EXPORT void CALLBACK motion_route_clr ();
EXPORT long CALLBACK motion_route_get_size ();
EXPORT void CALLBACK motion_route_start ();
EXPORT void CALLBACK motion_route_add (long TimeA,long TimeC,long TimeD,long Ax,long Ay,long Bx,long By,long RPx,long RPy,long Ar,long Az,long Br,long Bz,long RPr,long RPz);
EXPORT void CALLBACK motion_set_laseron_step(unsigned long ulStepPulse,unsigned long ulLaserONTime);
EXPORT void CALLBACK motion_set_home_index_ah(long sAxis,BOOL bAH);
EXPORT BOOL CALLBACK set_motion_index_delay(short sAxis,long lDelay);
EXPORT long CALLBACK motion_get_index_status(void);
EXPORT BOOL CALLBACK set_motion_encoder_parameters(short sAxis,short sInverse,double dlScale);
EXPORT BOOL CALLBACK set_motion_soft_limit(short sAxis,short sEnable,long lLimitP,long lLimitN);
/////////////////////////////////
//fly mark commands
//////////////////////////////////
EXPORT	void	CALLBACK set_fly_x_list (double dSpeed);
EXPORT	void	CALLBACK set_fly_y_list (double dSpeed);
EXPORT	void	CALLBACK set_ext_start_delay_list(long sEncoder, long sDelays);
EXPORT	void	CALLBACK simulate_encoder_list(unsigned long channel);
EXPORT void		CALLBACK	list_set_encoder_cnt(long encoder,long delay);
EXPORT void		CALLBACK	list_wait_encoder_cnt(void);
EXPORT void		CALLBACK	set_encoder_comp(long comp);
EXPORT void		CALLBACK	set_fly_preview_mode(long sEnable,long lXEnLmt,long lYEnLmt);
EXPORT double	CALLBACK	get_encoder_speed(long sEncoder,double dSpeedRate);
EXPORT void		CALLBACK	set_flyspeed_update_time(long lMaxTime,long lMinTime);
#endif
