#ifndef _V_DMA_H_
#define _V_DMA_H_

template <int> void AXIMMvideo2Bytes(
	const U8 videoFormat,
	V_MIX_AXIMM srcImg,
	V_MIX_AXIMM srcImg2,
	V_MIX_STREAM_PLANES& img,
	const U16 Height,
	const U16 WidthInBytes,
	const U16 StrideInBytes,
	bool enable);

template <int> void Bytes2MultiPixStream(
	V_MIX_STREAM_PLANES& srcImg,
	V_MIX_STREAM_MULTIPIX& img,
	V_MIX_STREAM_ALPHA& imgAlpha,
	U16 Height,
	U16 WidthIn,
	bool enable);

#endif
