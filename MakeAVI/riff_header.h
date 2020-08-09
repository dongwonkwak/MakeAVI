#pragma once

#include <stdint.h>
#include <variant>
#include <vector>


using WORD = uint16_t;
using DWORD = uint32_t;
using UINT = unsigned int;
using BYTE = unsigned char;
using FOURCC = DWORD;
struct Chunk;
struct List;

using List_or_Chunk = std::variant<Chunk*, List*>;


constexpr auto ckidSTREAMLIST = 0x6C727473;
constexpr auto ckidSTREAMHEADER = 0x68727473;
constexpr auto ckidVID = 0x73646976;
constexpr auto ckidMJPG2 = 0x67706A6D;
constexpr auto ckidMAINAVIHEADER = 0x68697661;
constexpr auto ckidHDRLHeader = 0x6C726468;
constexpr auto ckidMovi = 0x63643030;	// 00dc
constexpr auto chidMOVIHEADER = 0x69766F6D;		// movi
constexpr auto ckidRIFF = 0x46464952;		// RIFF
constexpr auto ckidLIST = 0x5453494c;		// LIST
constexpr auto ckidAVI = 0x20495641;		// AVI 
constexpr auto ckidMJPG = 0x47504A4D;
constexpr auto ckidSTRF = 0x66727473;

#pragma pack(push, 1)
struct Chunk
{
	FOURCC	ckID = 0;
	UINT	ckSize = 0;
};

struct List
{
	FOURCC	fcc;
	UINT	listSize = 0;
	FOURCC	listType = 0;
};


/* AVI Main header */
struct AVIMAINHEADER 
{
	FOURCC fcc;	// avih
	DWORD  cb;
	DWORD  dwMicroSecPerFrame;
	DWORD  dwMaxBytesPerSec;
	DWORD  dwPaddingGranularity;
	DWORD  dwFlags;
	DWORD  dwTotalFrames;
	DWORD  dwInitialFrames;
	DWORD  dwStreams;
	DWORD  dwSuggestedBufferSize;
	DWORD  dwWidth;
	DWORD  dwHeight;
	DWORD  dwReserved[4];
};

/* AVI stream header */
struct AVISTREAMHEADER 
{
	FOURCC  fcc; // strh
	DWORD   cb;
	FOURCC  fccType;
	FOURCC  fccHandler;
	DWORD   dwFlags;
	WORD    wPriority;
	WORD    wLanguage;
	DWORD   dwInitialFrames;
	DWORD   dwScale;
	DWORD   dwRate;
	DWORD   dwStart;
	DWORD   dwLength;
	DWORD   dwSuggestedBufferSize;
	DWORD   dwQuality;
	DWORD   dwSampleSize;

	struct rcFrame 
	{
		short   left;
		short   top;
		short   right;
		short   bottom;
	};
	rcFrame frame;
};

struct BITMAPINFOHEADER
{
	DWORD	bi_size;
	DWORD	bi_width;
	DWORD	bi_height;
	WORD	bi_planes;
	WORD	bi_bit_count;
	DWORD	bi_compression;
	DWORD	bi_size_image;
	DWORD	bi_x_pels_per_meter;
	DWORD	bi_y_pels_per_meter;
	DWORD	bi_clr_used;
	DWORD	bi_clr_important;
};

struct WAVEFORMATEX
{
	WORD	w_format_tag;
	WORD	n_channels;
	DWORD	n_samples_per_sec;
	DWORD	n_avg_bytes_per_sec;
	WORD	n_block_align;
	WORD	w_bits_per_sample;
	WORD	cb_size;
};

struct RIFF_Header
{
	FOURCC	fcc;
	UINT	fileSize = 0;
	FOURCC	fileType = 0;
};
#pragma pack(pop)