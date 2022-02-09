#ifndef _V_CSC_H_
#define _V_CSC_H_

typedef ap_uint<SAMPLES_PER_CLOCK*MAX_DATA_WIDTH>   Y_MEM_PIXEL_TYPE;
typedef ap_uint<SAMPLES_PER_CLOCK*MAX_DATA_WIDTH>   C_MEM_PIXEL_TYPE;

template <bool> void v_mix_yuv2rgb(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16 height,
	U16 width,
	bool layerEnableFlag,
#if(ENABLE_CSC_COEFFICIENT_REGISTERS == 1)
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
#endif
	V_MIX_STREAM_MULTIPIX& outImg);

template <bool> void v_mix_rgb2yuv(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16& height,
		U16& width,
#if(ENABLE_CSC_COEFFICIENT_REGISTERS == 1)
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
	V_MIX_STREAM_MULTIPIX& outImg);

template <bool> void v_mix_420_to_422(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16 height,
	U16 width,
	bool layerEnableFlag,
	V_MIX_STREAM_MULTIPIX& outImg);

template <bool> void v_mix_422_to_444(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16 height,
	U16 width,
	bool layerEnableFlag,
	V_MIX_STREAM_MULTIPIX& outImg);

template <bool> void v_mix_444_to_422(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16& height,
	U16& width,
	V_MIX_STREAM_MULTIPIX& outImg);

template <bool> void v_mix_422_to_420(
	V_MIX_STREAM_MULTIPIX& srcImg,
	U16& height,
	U16& width,
	V_MIX_STREAM_MULTIPIX& outImg);

#endif
