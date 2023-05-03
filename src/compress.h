#ifndef COMPRESS_H
#define COMPRESS_H

#include <vector>

void compressData(const std::vector<char>& inputData, std::vector<char>& outputData);
void decompressData(const std::vector<char>& inputData, std::vector<char>& outputData);

#endif
