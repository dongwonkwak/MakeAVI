#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <numeric>
#include "riff_header.h"
namespace fs = std::filesystem;

/*

	RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)
            LIST ('strl'
                  'strh'(<Stream header>)
                  'strf'(<Stream format>)
                  [ 'strd'(<Additional header data>) ]
                  [ 'strn'(<Stream name>) ]
                  ...
                 )
             ...
           )

*/


UINT get_movi_list_size(const std::string& filepath)
{
	UINT ret = 0;
	
	for (const auto& p : std::filesystem::directory_iterator(filepath))
	{
		if (fs::is_regular_file(p) && p.path().extension() == ".jpg")
		{
			auto tmp = sizeof(Chunk) + p.file_size();
			ret += (tmp + (p.file_size() % sizeof(WORD)));
		}
	}

	return ret;
}

int main(int argc, char* argv[])
{
	UINT total_size = 0;
	DWORD total_frame = 0;
	const DWORD width = 724;
	const DWORD height = 1046;
	const size_t movi_pos = 212;
	
	std::string filepath = "./testimg";
	UINT movi_size = get_movi_list_size(filepath);
	std::cout << movi_size << std::endl;
	total_size = 224 + movi_size;
	std::cout << "file size: " << total_size << std::endl;
	std::vector<BYTE> buf(total_size, 0x0);

	// 1. make movi list header
	List movi_header{ ckidLIST, movi_size, chidMOVIHEADER };
	memcpy(&buf[movi_pos], &movi_header, sizeof(List));
	
	size_t current_pos = 224;
	for (const auto& p : std::filesystem::directory_iterator(filepath))
	{
		if (fs::is_regular_file(p) && p.path().extension() == ".jpg")
		{
			// 2. make movi chunk
			Chunk c{ ckidMovi, p.file_size() };
			memcpy(&buf[current_pos], &c, sizeof(Chunk));
			current_pos += sizeof(Chunk);
			std::ifstream ifs(p.path().string(), std::ios::in | std::ios::binary);
			ifs.read((char*)&buf[current_pos], p.file_size());
			current_pos += p.file_size() + (p.file_size() % sizeof(WORD));
			ifs.close();
			total_frame++;
		}
	}

	RIFF_Header riff_header{ ckidRIFF, movi_size + 216,  ckidAVI };
	memcpy(&buf[0], &riff_header, sizeof(RIFF_Header));
	auto offset = sizeof(RIFF_Header);
	
	// 3. make RIFF header
	List hdrl{ ckidLIST, 192, ckidHDRLHeader };	// 192
	memcpy(&buf[offset], &hdrl, sizeof(List));
	offset += sizeof(List);

	// 4. make avi main header
	AVIMAINHEADER avi_main_header{	// OK
		ckidMAINAVIHEADER,	// avih
		sizeof(AVIMAINHEADER) - 8,
		33333, 
		16777216, 
		0, 
		0, 
		total_frame, 
		0, 
		1, 
		16777216, 
		width, height
	};
	memcpy(&buf[offset], &avi_main_header, sizeof(AVIMAINHEADER));
	offset += sizeof(AVIMAINHEADER);

	List datastream{ ckidLIST, sizeof(AVISTREAMHEADER) + sizeof(BITMAPINFOHEADER) + 12, ckidSTREAMLIST };
	memcpy(&buf[offset], &datastream, sizeof(List));
	offset += sizeof(List);

	// 64
	AVISTREAMHEADER stream_header{
		ckidSTREAMHEADER,	// strh
		sizeof(AVISTREAMHEADER) - 8,	// 56
		ckidVID,
		ckidMJPG2,
		0, 0, 0, 0,
		33333,
		1000000,	// dwrate
		0, total_frame, 16777216,
		10000, 0,
		0, 0, 724, 1046
	};
	memcpy(&buf[offset], &stream_header, sizeof(AVISTREAMHEADER));
	offset += sizeof(AVISTREAMHEADER);
	memcpy(&buf[offset], &ckidSTRF, 4);
	offset += 4;
	UINT stream_format_size = sizeof(BITMAPINFOHEADER);
	memcpy(&buf[offset], &stream_format_size, 4);
	offset += 4;

	// 48
	BITMAPINFOHEADER stream_format{
		sizeof(BITMAPINFOHEADER),	// 40
		724, 1046,
		1, 24, ckidMJPG,
		757304, // biSizeImage: 757304
		0, 0, 0, 0
	};
	memcpy(&buf[offset], &stream_format, sizeof(BITMAPINFOHEADER));
	//offset += sizeof(BITMAPINFOHEADER);

	std::ofstream ofs("test.avi", std::ios::binary);
	ofs.write((char*)&buf[0], buf.size());
	ofs.close();
	
	return 0;
}