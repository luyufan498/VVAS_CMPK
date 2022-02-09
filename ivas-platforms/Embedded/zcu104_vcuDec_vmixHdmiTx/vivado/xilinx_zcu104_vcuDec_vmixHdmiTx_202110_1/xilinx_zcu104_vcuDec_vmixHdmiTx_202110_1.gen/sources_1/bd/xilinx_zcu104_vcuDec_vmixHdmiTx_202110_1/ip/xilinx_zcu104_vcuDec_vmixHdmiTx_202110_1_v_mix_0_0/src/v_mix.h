#ifndef _V_MIX_TOP_H_
#define _V_MIX_TOP_H_

#include "hls_video.h"
#include "ap_int.h"

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;

typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;

#define ENABLE_VITIS_HLS	1

// Streaming video formats
#define RGB444					0
#define YUV444					1
#define YUV422 					2
#define YUV420				    3
#define Y                       4
#define RGBA                    5
#define YUVA444                 6

// Video in memory formats
#define RGBX8					10	// [31:0] x:B:G:R 8:8:8:8
#define YUVX8					11  // [31:0] x:V:U:Y 8:8:8:8
#define YUYV8					12	// [31:0] V:Y:U:Y 8:8:8:8
#define RGBA8					13	// [31:0] A:B:G:R 8:8:8:8
#define YUVA8					14	// [31:0] A:V:U:Y 8:8:8:8
#define RGBX10					15	// [31:0] x:B:G:R 2:10:10:10
#define YUVX10					16	// [31:0] x:V:U:Y 2:10:10:10
#define RGB565				    17  // [15:0] B:G:R 5:6:5
#define Y_UV8					18	// [15:0] Y:Y 8:8, [15:0] V:U 8:8
#define Y_UV8_420				19	// [15:0] Y:Y 8:8, [15:0] V:U 8:8
#define RGB8					20  // [23:0] B:G:R 8:8:8
#define YUV8					21  // [24:0] V:U:Y 8:8:8
#define Y_UV10					22	// [31:0] x:Y:Y:Y 2:10:10:10, [31:0] x:U:V:U 2:10:10:10
#define Y_UV10_420				23	// [31:0] x:Y:Y:Y 2:10:10:10, [31:0] x:U:V:U 2:10:10:10
#define Y8                      24  // [31:0] Y3:Y2:Y1:Y0 8:8:8:8
#define Y10                     25  // [31:0] x:Y2:Y1:Y0 2:10:10:10
#define BGRA8					26	// [31:0] A:R:G:B 8:8:8:8
#define BGRX8                   27  // [31:0] X:R:G:B 8:8:8:8
#define UYVY8                   28  // [31:0] Y:V:Y:U 8:8:8:8
#define BGR8					29  // [23:0] R:G:B 8:8:8

#define IS_PIXEL_ALPHA(a)		((a)==RGBA8 || (a)==YUVA8 || (a)==BGRA8 || (a)==RGBA || (a)==YUVA444)
#define IS_YUV(a)				((a)==YUV444 || (a)==YUVA444 || (a)==YUV422 || (a)==YUV420 || (a)==YUVX8 || (a)==YUYV8 || (a)==UYVY8 || (a)==YUVA8 || (a)==YUVX10 || (a)==Y_UV8 || (a)==Y_UV8_420 || (a)==YUV8 || (a)==Y_UV10 || (a)==Y_UV10_420 || (a)==Y8 || (a)==Y10)
#define IS_422(a)				((a)==YUV422 || (a)==YUYV8 || (a)==Y_UV8 || (a)==Y_UV10 || (a)==UYVY8)
#define NR_PLANES(a) 			(((a)==Y_UV8 || (a)==Y_UV8_420 || (a)==Y_UV10 || (a)==Y_UV10_420) ? 2 :1)
#define IS_420(a)				((a)==YUV420 || (a)==Y_UV8_420 || (a)==Y_UV10_420)
#define DEPTH(a)				(((a)==RGBX10 || (a)==YUVX10 || (a)==Y_UV10 || (a)==Y_UV10_420 || (a)==Y10) ? 10 : 8)

const int BYTESxPIXEL[] =
{
	// Streaming video formats
	0, // RGB444
	0, // YUV444
	0, // YUV422
	0, // YUV420
	0, // Y
	0, // RGBA444
	0, // YUVA444
	0, // unused
	0, // unused
	0, // unused

	// Video in memory formats
	4, // RGBX8
	4, // YUVX8
	2, // YUYV8
	4, // RGBA8
	4, // YUVA8
	4, // RGBX10
	4, // YUVX10
	2, // RGB565
	1, // Y_UV8
	1, // Y_UV8_420
	3, // RGB8
	3, // YUV8
	4, // Y_UV10        4 bytes per 3 pixels
	4, // Y_UV10_420    4 bytes per 3 pixels
	1, // Y8
	4, // Y10           4 bytes per 3 pixels
	4, // BGRA8
    4, // BGRX8
    2, // UYVY8
	3  // BGR8
};

typedef ap_uint<MAX_DATA_WIDTH> PIXEL_TYPE;
typedef hls::Scalar<NR_COMPONENTS, PIXEL_TYPE> YUV_PIXEL;
typedef hls::Scalar<NR_COMPONENTS*SAMPLES_PER_CLOCK, PIXEL_TYPE>  YUV_MULTI_PIXEL;
typedef hls::Scalar<SAMPLES_PER_CLOCK, U8> MULTI_ALPHA;

typedef ap_uint<NR_COMPONENTS*SAMPLES_PER_CLOCK*MAX_DATA_WIDTH>   MEM_PIXEL_TYPE;
typedef ap_uint<SAMPLES_PER_CLOCK*8>                              MEM_ALPHA_TYPE;
typedef ap_uint<SAMPLES_PER_CLOCK*MAX_DATA_WIDTH>                 MEM_ALPHA_TYPE2;

typedef hls::stream<ap_axiu<BITS_PER_CLOCK,1,1,1> >                       V_MIX_AXI_STREAM_IN;
typedef hls::stream<ap_axiu<4*MAX_DATA_WIDTH*SAMPLES_PER_CLOCK,1,1,1> >   V_MIX_AXI_STREAM_ALPHA_IN;
typedef hls::stream<ap_axiu<BITS_PER_CLOCK,1,1,1> >                       V_MIX_AXI_STREAM_OUT;
typedef hls::stream<YUV_MULTI_PIXEL>                                      V_MIX_STREAM_MULTIPIX;
typedef hls::stream<MULTI_ALPHA>                   						  V_MIX_STREAM_ALPHA;
typedef hls::stream<ap_uint<AXIMM_DATA_WIDTH> >                       	  V_MIX_STREAM_BYTES;

typedef struct {
	V_MIX_STREAM_BYTES plane0;
	V_MIX_STREAM_BYTES plane1;
} V_MIX_STREAM_PLANES;

typedef ap_uint<AXIMM_DATA_WIDTH>* V_MIX_AXIMM;

//HW Registers
typedef struct mix_hw_struct
{
	U16 width;
	U16 height;
	U16 video_format;
    U16 background_Y_R;
    U16 background_U_G;
    U16 background_V_B;
	//U16 layerEnable;
	U32 layerEnable;
#if (NR_LAYERS>1 && LAYER1_INTF_TYPE==0)
	V_MIX_AXIMM layer1_buf1;
	V_MIX_AXIMM layer1_buf2;
#endif
#if (NR_LAYERS>2 && LAYER2_INTF_TYPE==0)
	V_MIX_AXIMM layer2_buf1;
	V_MIX_AXIMM layer2_buf2;
#endif
#if (NR_LAYERS>3 && LAYER3_INTF_TYPE==0)
	V_MIX_AXIMM layer3_buf1;
	V_MIX_AXIMM layer3_buf2;
#endif
#if (NR_LAYERS>4 && LAYER4_INTF_TYPE==0)
	V_MIX_AXIMM layer4_buf1;
	V_MIX_AXIMM layer4_buf2;
#endif
#if (NR_LAYERS>5 && LAYER5_INTF_TYPE==0)
	V_MIX_AXIMM layer5_buf1;
	V_MIX_AXIMM layer5_buf2;
#endif
#if (NR_LAYERS>6 && LAYER6_INTF_TYPE==0)
	V_MIX_AXIMM layer6_buf1;
	V_MIX_AXIMM layer6_buf2;
#endif
#if (NR_LAYERS>7 && LAYER7_INTF_TYPE==0)
	V_MIX_AXIMM layer7_buf1;
	V_MIX_AXIMM layer7_buf2;
#endif
#if (NR_LAYERS>8 && LAYER8_INTF_TYPE==0)
	V_MIX_AXIMM layer8_buf1;
	V_MIX_AXIMM layer8_buf2;
#endif
#if (NR_LAYERS>9 && LAYER9_INTF_TYPE==0)
	V_MIX_AXIMM layer9_buf1;
	V_MIX_AXIMM layer9_buf2;
#endif
#if (NR_LAYERS>10 && LAYER10_INTF_TYPE==0)
	V_MIX_AXIMM layer10_buf1;
	V_MIX_AXIMM layer10_buf2;
#endif
#if (NR_LAYERS>11 && LAYER11_INTF_TYPE==0)
	V_MIX_AXIMM layer11_buf1;
	V_MIX_AXIMM layer11_buf2;
#endif
#if (NR_LAYERS>12 && LAYER12_INTF_TYPE==0)
	V_MIX_AXIMM layer12_buf1;
	V_MIX_AXIMM layer12_buf2;
#endif
#if (NR_LAYERS>13 && LAYER13_INTF_TYPE==0)
	V_MIX_AXIMM layer13_buf1;
	V_MIX_AXIMM layer13_buf2;
#endif
#if (NR_LAYERS>14 && LAYER14_INTF_TYPE==0)
	V_MIX_AXIMM layer14_buf1;
	V_MIX_AXIMM layer14_buf2;
#endif
#if (NR_LAYERS>15 && LAYER15_INTF_TYPE==0)
	V_MIX_AXIMM layer15_buf1;
	V_MIX_AXIMM layer15_buf2;
#endif
#if (NR_LAYERS>16 && LAYER16_INTF_TYPE==0)
	V_MIX_AXIMM layer16_buf1;
	V_MIX_AXIMM layer16_buf2;
#endif

	bool layerEnableFlag[NR_LAYERS];
	U16 layerAlpha[NR_LAYERS];
	U16 layerStartX[NR_LAYERS];
	U16 layerStartY[NR_LAYERS];
	U16 layerWidth[NR_LAYERS];
	U16 layerHeight[NR_LAYERS];
	U8  layerScaleFactor[NR_LAYERS];
	U8  layerVideoFormat[NR_LAYERS];
#if (LOGO_LAYER)
	U16 logoStartX;
	U16 logoStartY;
	U16 logoWidth;
	U16 logoHeight;
	U16 logoScaleFactor;
	U16 logoAlpha;
    ap_uint<8*SAMPLES_PER_CLOCK> logoR[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK];
    ap_uint<8*SAMPLES_PER_CLOCK> logoG[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK];
    ap_uint<8*SAMPLES_PER_CLOCK> logoB[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK];
#if (LOGO_PIXEL_ALPHA)
    ap_uint<8*SAMPLES_PER_CLOCK> logoA[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK];
#endif
#endif
	U16 layerStride[NR_LAYERS];

#if (LOGO_LAYER && LOGO_TRANSPARENCY_COLOR)
    U8 logoClrKeyMin_R;
    U8 logoClrKeyMin_G;
    U8 logoClrKeyMin_B;
    U8 logoClrKeyMax_R;
    U8 logoClrKeyMax_G;
    U8 logoClrKeyMax_B;
#endif
	U16 reserve;
#ifdef ENABLE_CSC_COEFFICIENT_REGISTERS
	/* For yuv to rgb */
    I32 K11;
    I32 K12;
    I32 K13;
    I32 K21;
    I32 K22;
    I32 K23;
    I32 K31;
    I32 K32;
    I32 K33;
    I32 ROffset;
    I32 GOffset;
    I32 BOffset;

    /* For rgb to yuv */
    I32 K11_2;
    I32 K12_2;
    I32 K13_2;
    I32 K21_2;
    I32 K22_2;
    I32 K23_2;
    I32 K31_2;
    I32 K32_2;
    I32 K33_2;
    I32 YOffset;
    I32 UOffset;
    I32 VOffset;
#endif
}V_MIX_HW_STRUCT_REG;

// top level function for HW synthesis
extern void v_mix(
		V_MIX_AXI_STREAM_IN& s_axis_video,
#if (NR_LAYERS>1 && LAYER1_INTF_TYPE==1)
#if LAYER1_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video1,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video1,
#endif
#endif
#if (NR_LAYERS>2 && LAYER2_INTF_TYPE==1)
#if LAYER2_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video2,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video2,
#endif
#endif
#if (NR_LAYERS>3 && LAYER3_INTF_TYPE==1)
#if LAYER3_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video3,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video3,
#endif
#endif
#if (NR_LAYERS>4 && LAYER4_INTF_TYPE==1)
#if LAYER4_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video4,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video4,
#endif
#endif
#if (NR_LAYERS>5 && LAYER5_INTF_TYPE==1)
#if LAYER5_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video5,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video5,
#endif
#endif
#if (NR_LAYERS>6 && LAYER6_INTF_TYPE==1)
#if LAYER6_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video6,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video6,
#endif
#endif
#if (NR_LAYERS>7 && LAYER7_INTF_TYPE==1)
#if LAYER7_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video7,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video7,
#endif
#endif
#if (NR_LAYERS>8 && LAYER8_INTF_TYPE==1)
#if LAYER8_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video8,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video8,
#endif
#endif
#if (NR_LAYERS>9 && LAYER9_INTF_TYPE==1)
#if LAYER9_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video9,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video9,
#endif
#endif
#if (NR_LAYERS>10 && LAYER10_INTF_TYPE==1)
#if LAYER10_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video10,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video10,
#endif
#endif
#if (NR_LAYERS>11 && LAYER11_INTF_TYPE==1)
#if LAYER11_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video11,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video11,
#endif
#endif
#if (NR_LAYERS>12 && LAYER12_INTF_TYPE==1)
#if LAYER12_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video12,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video12,
#endif
#endif
#if (NR_LAYERS>13 && LAYER13_INTF_TYPE==1)
#if LAYER13_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video13,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video13,
#endif
#endif
#if (NR_LAYERS>14 && LAYER14_INTF_TYPE==1)
#if LAYER14_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video14,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video14,
#endif
#endif
#if (NR_LAYERS>15 && LAYER15_INTF_TYPE==1)
#if LAYER15_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video15,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video15,
#endif
#endif
#if (NR_LAYERS>16 && LAYER16_INTF_TYPE==1)
#if LAYER16_PIXEL_ALPHA
		V_MIX_AXI_STREAM_ALPHA_IN& s_axis_video16,
#else
		V_MIX_AXI_STREAM_IN& s_axis_video16,
#endif
#endif
#if ENABLE_VITIS_HLS
		U16 width,
		U16 height,
		U16 video_format,
		U16 background_Y_R,
		U16 background_U_G,
		U16 background_V_B,
		//U16 layerEnable,
		U32 layerEnable,
#if (NR_LAYERS>1 && LAYER1_INTF_TYPE==0)
		V_MIX_AXIMM layer1_buf1,
		V_MIX_AXIMM layer1_buf2,
#endif
#if (NR_LAYERS>2 && LAYER2_INTF_TYPE==0)
		V_MIX_AXIMM layer2_buf1,
		V_MIX_AXIMM layer2_buf2,
#endif
#if (NR_LAYERS>3 && LAYER3_INTF_TYPE==0)
		V_MIX_AXIMM layer3_buf1,
		V_MIX_AXIMM layer3_buf2,
#endif
#if (NR_LAYERS>4 && LAYER4_INTF_TYPE==0)
		V_MIX_AXIMM layer4_buf1,
		V_MIX_AXIMM layer4_buf2,
#endif
#if (NR_LAYERS>5 && LAYER5_INTF_TYPE==0)
		V_MIX_AXIMM layer5_buf1,
		V_MIX_AXIMM layer5_buf2,
#endif
#if (NR_LAYERS>6 && LAYER6_INTF_TYPE==0)
		V_MIX_AXIMM layer6_buf1,
		V_MIX_AXIMM layer6_buf2,
#endif
#if (NR_LAYERS>7 && LAYER7_INTF_TYPE==0)
		V_MIX_AXIMM layer7_buf1,
		V_MIX_AXIMM layer7_buf2,
#endif
#if (NR_LAYERS>8 && LAYER8_INTF_TYPE==0)
		V_MIX_AXIMM layer8_buf1,
		V_MIX_AXIMM layer8_buf2,
#endif
#if (NR_LAYERS>9 && LAYER9_INTF_TYPE==0)
		V_MIX_AXIMM layer9_buf1,
		V_MIX_AXIMM layer9_buf2,
#endif
#if (NR_LAYERS>10 && LAYER10_INTF_TYPE==0)
		V_MIX_AXIMM layer10_buf1,
		V_MIX_AXIMM layer10_buf2,
#endif
#if (NR_LAYERS>11 && LAYER11_INTF_TYPE==0)
		V_MIX_AXIMM layer11_buf1,
		V_MIX_AXIMM layer11_buf2,
#endif
#if (NR_LAYERS>12 && LAYER12_INTF_TYPE==0)
		V_MIX_AXIMM layer12_buf1,
		V_MIX_AXIMM layer12_buf2,
#endif
#if (NR_LAYERS>13 && LAYER13_INTF_TYPE==0)
		V_MIX_AXIMM layer13_buf1,
		V_MIX_AXIMM layer13_buf2,
#endif
#if (NR_LAYERS>14 && LAYER14_INTF_TYPE==0)
		V_MIX_AXIMM layer14_buf1,
		V_MIX_AXIMM layer14_buf2,
#endif
#if (NR_LAYERS>15 && LAYER15_INTF_TYPE==0)
		V_MIX_AXIMM layer15_buf1,
		V_MIX_AXIMM layer15_buf2,
#endif
#if (NR_LAYERS>16 && LAYER16_INTF_TYPE==0)
		V_MIX_AXIMM layer16_buf1,
		V_MIX_AXIMM layer16_buf2,
#endif

		U16 layerAlpha[NR_LAYERS],
		U16 layerStartX[NR_LAYERS],
		U16 layerStartY[NR_LAYERS],
		U16 layerWidth[NR_LAYERS],
		U16 layerHeight[NR_LAYERS],
		U8  layerScaleFactor[NR_LAYERS],
		U8  layerVideoFormat[NR_LAYERS],
#if (LOGO_LAYER)
		U16 logoStartX,
		U16 logoStartY,
		U16 logoWidth,
		U16 logoHeight,
		U16 logoScaleFactor,
		U16 logoAlpha,
		ap_uint<8*SAMPLES_PER_CLOCK> logoR[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK],
		ap_uint<8*SAMPLES_PER_CLOCK> logoG[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK],
		ap_uint<8*SAMPLES_PER_CLOCK> logoB[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK],
#if (LOGO_PIXEL_ALPHA)
		ap_uint<8*SAMPLES_PER_CLOCK> logoA[MAX_LOGO_WIDTH*MAX_LOGO_HEIGHT/SAMPLES_PER_CLOCK],
#endif
#endif
		U16 *layerStride,
#if (LOGO_LAYER && LOGO_TRANSPARENCY_COLOR)
		U8 logoClrKeyMin_R,
		U8 logoClrKeyMin_G,
		U8 logoClrKeyMin_B,
		U8 logoClrKeyMax_R,
		U8 logoClrKeyMax_G,
		U8 logoClrKeyMax_B,
#endif
		U16 reserve,
#ifdef ENABLE_CSC_COEFFICIENT_REGISTERS
		/* For yuv to rgb */
		I32 K11,
		I32 K12,
		I32 K13,
		I32 K21,
		I32 K22,
		I32 K23,
		I32 K31,
		I32 K32,
		I32 K33,
		I32 ROffset,
		I32 GOffset,
		I32 BOffset,

		/* For rgb to yuv */
		I32 K11_2,
		I32 K12_2,
		I32 K13_2,
		I32 K21_2,
		I32 K22_2,
		I32 K23_2,
		I32 K31_2,
		I32 K32_2,
		I32 K33_2,
		I32 YOffset,
		I32 UOffset,
		I32 VOffset,
#endif
#else /*ENABLE_VITIS_HLS*/
		V_MIX_HW_STRUCT_REG HwReg,
#endif
		V_MIX_AXI_STREAM_OUT& m_axis_video
);

#endif
