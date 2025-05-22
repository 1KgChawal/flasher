#include "file.h++"

inline bool is_usb_device(const std::string &path) {
    std::string dev_name = std::filesystem::path(path).filename();
    std::string sys_path = "/sys/block/" + dev_name + "/device";

    std::filesystem::path p = std::filesystem::canonical(sys_path);
    return p.string().find("/usb") != std::string::npos;
}

inline size_t get_size(const std::string &path) {
    return std::filesystem::file_size(path);
}

void print_progress(long long size_written, long long input_size) {
    constexpr long long bars = 50;
    std::cout << "\r[";
    int pos = (bars * size_written) / input_size;
    for (int i = 0; i < bars; i++) {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << ((static_cast<float>(size_written))/input_size)*100 << " %" << std::flush;
}

USBdrive_flasher::USBdrive_flasher(const std::string &dest, const std::string &src) : size(1024 * 4 * 8), buffer(std::make_unique<char[]>(size)) {
    if (!std::filesystem::exists(src) || !std::filesystem::exists(dest) || std::filesystem::is_directory(src)) {
        throw std::runtime_error("incorrect dest or src");
    }
    if (!std::filesystem::is_block_file(dest)) {
        throw std::runtime_error("dest is not a block file");
    }
    if (!is_usb_device(dest)) {
        throw std::runtime_error("specified dest is not a USB device");
    }
    fout.open(dest, std::ios::binary);
    if (!fout) {
        throw std::runtime_error("unable to open dest");
    }
    fin.open(src, std::ios::binary);
    if (!fin) {
        throw std::runtime_error("unable to open src");
    }
    input_size = get_size(src);
}

size_t USBdrive_flasher::initiate_flashing() {
    std::cout << "Do you want to initiate flashing? [y/N]";
    std::string input;
    std::getline(std::cin, input);
    if (input.size() != 1 && tolower(input[0]) != 'y') {
        std::cout << "Exitting flashing" << std::endl;
        return 0;
    }
    size_t bytes_read = 0;
    std::cout << "Initiating flashing\n";
    std::cout << "Flashing " << input_size/1024 << " KB of data" << std::endl; 
    while (true) {
        fin.read(buffer.get(), size);
        size_t size_read = fin.gcount();
        if (size_read == 0) {
            break;
        }
        bytes_read += size_read;
        fout.write(buffer.get(), size_read);
        if (!fout) {
            throw std::runtime_error("error occured during runtime flashing");
        }
        print_progress(bytes_read, input_size);
        if(bytes_read % 1024*1024 == 0){
            fout.flush();
        }
    }
    std::cout << "\nFlashing completed" << std::endl;
    return bytes_read;
}