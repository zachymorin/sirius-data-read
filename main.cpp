#include <iostream>
#include <fstream>
#include "framing/can_header.hpp"
#include "telemetry/telemetry_type.hpp"
#include <cstring>
#include <string>

void parseSDData(const std::string& binaryPath, const std::string& csvPath) {
    std::ifstream inFile(binaryPath, std::ios::binary);
    std::ofstream outFile(csvPath);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Failed to open input or output files.\n";
        return;
    }

    // Write CSV Columns
    outFile << "Priority,SenderID,TargetID,PayloadType,PayloadID,Seq,ErrorCode\n";

    CanHeader header;
    size_t count = 0;

    // Read 4-byte raw CAN IDs continuously
    while (inFile.read(reinterpret_cast<char*>(&header.code), sizeof(header.code))) {
        
        outFile << header.frame.priority << ","
                << header.frame.sender_id << ","
                << header.frame.target_id << ","
                << header.frame.payload_type << ","
                << header.frame.payload_id << ","
                << header.frame.seq << ","
                << header.frame.error_code << "\n";

        count++;
    }

    std::cout << "Successfully parsed " << count << " frames into " << csvPath << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No default binary file path provided. Exiting..." << std::endl;
        return 1; 
    }

    if (std::strstr(argv[1], ".bin") == nullptr){
        std::cout << "Path provided is not a binary file" << std::endl;
        return 1; 
    }

    std::string outputPath = "output/output.csv";

    if(argc > 2){
        std::cout << "Using provided output path" << std::endl;
        outputPath = argv[2]; 
    }

    parseSDData(argv[1], outputPath); 
    return 0;
}