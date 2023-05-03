#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <zlib.h>

using namespace std;

void printUsage() {
    cerr << "Usage: compressor [-c|-d] input output. -c 'file to be compressed' filename | -d 'file to be decompressed' filename" << endl;
}

void compressFile(const string& inputFilename, const string& outputFilename) {
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile) {
        throw runtime_error("Failed to open input file");
    }

    ofstream outputFile(outputFilename, ios::binary);
    if (!outputFile) {
        throw runtime_error("Failed to open output file");
    }

    const int bufferSize = 128 * 1024;
    char buffer[bufferSize];

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        throw runtime_error("Failed to initialize compression stream");
    }

    while (inputFile.good()) {
        inputFile.read(buffer, bufferSize);
        stream.avail_in = static_cast<uInt>(inputFile.gcount());
        stream.next_in = reinterpret_cast<Bytef*>(buffer);

        do {
            stream.avail_out = bufferSize;
            stream.next_out = reinterpret_cast<Bytef*>(buffer);
            deflate(&stream, Z_FINISH);
            outputFile.write(buffer, bufferSize - stream.avail_out);
        } while (stream.avail_out == 0);
    }

    deflateEnd(&stream);
}

void decompressFile(const string& inputFilename, const string& outputFilename) {
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile) {
        throw runtime_error("Failed to open input file");
    }

    ofstream outputFile(outputFilename, ios::binary);
    if (!outputFile) {
        throw runtime_error("Failed to open output file");
    }

    const int bufferSize = 128 * 1024;
    char buffer[bufferSize];

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    if (inflateInit(&stream) != Z_OK) {
        throw runtime_error("Failed to initialize decompression stream");
    }

    while (inputFile.good()) {
        inputFile.read(buffer, bufferSize);
        stream.avail_in = static_cast<uInt>(inputFile.gcount());
        stream.next_in = reinterpret_cast<Bytef*>(buffer);

        do {
            stream.avail_out = bufferSize;
            stream.next_out = reinterpret_cast<Bytef*>(buffer);
            inflate(&stream, Z_NO_FLUSH);
            outputFile.write(buffer, bufferSize - stream.avail_out);
        } while (stream.avail_out == 0);
    }

    inflateEnd(&stream);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage();
        return 1;
    }

    string mode = argv[1];
    string inputFilename = argv[2];
    string outputFilename = argv[3];

    if (mode == "-c") {
        compressFile(inputFilename, outputFilename);
    } else if (mode == "-d") {
        decompressFile(inputFilename, outputFilename);
    } else {
        printUsage();
        return 1;
    }

    return 0;
}
