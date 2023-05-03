#include "compress.h"
#include <zlib.h>

void compressData(const std::vector<char>& inputData, std::vector<char>& outputData) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    deflateInit(&stream, Z_DEFAULT_COMPRESSION);

    std::vector<char> buffer(1024);

    stream.avail_in = inputData.size();
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(inputData.data()));

    do {
        buffer.resize(buffer.size() * 2);
        stream.avail_out = buffer.size() - stream.total_out;
        stream.next_out = reinterpret_cast<Bytef*>(buffer.data() + stream.total_out);
        deflate(&stream, inputData.empty() ? Z_FINISH : Z_NO_FLUSH);
    } while (stream.avail_out == 0);

    outputData.resize(stream.total_out);
    std::copy(buffer.begin(), buffer.begin() + stream.total_out, outputData.begin());

    deflateEnd(&stream);
}

void decompressData(const std::vector<char>& inputData, std::vector<char>& outputData) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    inflateInit(&stream);

    std::vector<char> buffer(1024);

    stream.avail_in = inputData.size();
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(inputData.data()));

    do {
        buffer.resize(buffer.size() * 2);
        stream.avail_out = buffer.size() - stream.total_out;
        stream.next_out = reinterpret_cast<Bytef*>(buffer.data() + stream.total_out);
        inflate(&stream, Z_NO_FLUSH);
    } while (stream.avail_out == 0);

    outputData.resize(stream.total_out);
    std::copy(buffer.begin(), buffer.begin() + stream.total_out, outputData.begin());

    inflateEnd(&stream);
}
