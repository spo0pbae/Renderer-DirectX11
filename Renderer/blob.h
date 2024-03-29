#ifndef BLOB_H
#define BLOB_H

#include <cstdint>
#include <vector>

namespace end
{
	using binary_blob_t = std::vector<uint8_t>;
	binary_blob_t load_binary_blob(const char* path);
}// namespace end

#endif