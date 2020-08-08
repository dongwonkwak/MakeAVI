#pragma once

#include <stdint.h>
#include <variant>
#include <vector>

using BYTE = unsigned char;
using CKSIZE = uint32_t;
using FOURCC = uint32_t;
struct Chunk;
struct List;

using List_or_Chunk = std::vector<std::variant<Chunk*, List*>>;

#pragma pack(push, 1)
struct Chunk
{
	FOURCC ckID;
	CKSIZE ckSize;
	BYTE* ckData = nullptr;
};

struct List
{
	const FOURCC header = ('L' << 24 & 'I' << 16 & 'S' << 8 & 'T');
	CKSIZE listSize;
	FOURCC listType;
	List_or_Chunk listData;
};

struct RIFF_Header
{
	const FOURCC header = ('R' << 24 & 'I' << 16 & 'F' << 8 & 'F');
	CKSIZE fileSize = 0;
	FOURCC fileType;
	List_or_Chunk data;
};
#pragma pack(pop)