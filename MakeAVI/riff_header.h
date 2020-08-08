#pragma once

#include <stdint.h>
#include <variant>
#include <vector>

using WORD = uint16_t;
using DWORD = uint32_t;
using BYTE = unsigned char;
using UINT = uint32_t;
using FOURCC = uint32_t;
struct Chunk;
struct List;

using List_or_Chunk = std::variant<Chunk*, List*>;
#define FCC(x) \
    ((((DWORD)(x) & 0x000000FF) << 24) | (((DWORD)(x) & 0x0000FF00) << 8) | \
    (((DWORD)(x) & 0x00FF0000) >> 8) | (((DWORD)(x) && 0xFF000000) >> 24))

#pragma pack(push, 1)
struct Chunk
{
	FOURCC	ckID = 0;
	UINT	ckSize = 0;
	BYTE*	ckData = nullptr;
};

struct List
{
	FOURCC	fcc;
	UINT	listSize = 0;
	FOURCC	listType = 0;
	std::vector<List_or_Chunk> listData;
};


/* AVI Main header */
struct AVIMAINHEADER {
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
struct AVISTREAMHEADER {
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
} ;


struct BITMAPINFOHEADER
{
	DWORD	bi_size;
	DWORD	bi_width;
	DWORD	bi_height;
	WORD	bi_planes;
	WORD	bi_bit_count;
	DWORD	bi_compression;
	DWORD	bi_size_image;
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
	FOURCC	ckID;
	UINT	fileSize = 0;
	FOURCC	fileType = 0;
	std::vector<List_or_Chunk> data;
};
#pragma pack(pop)