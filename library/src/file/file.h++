#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class USBdrive_flasher {
   private:
    std::ofstream fout;
    std::ifstream fin;
    size_t size;
    std::unique_ptr<char[]> buffer;
    size_t input_size;
    int fd_out;

   public:
    USBdrive_flasher(const std::string &dest, const std::string &src);
    size_t initiate_flashing();
    ~USBdrive_flasher();
};