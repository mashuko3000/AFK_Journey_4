//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_MEMORY_HPP
#define EMAIL_MEMORY_HPP

#include<string_view>
#include<string>
#include<fcntl.h>
#include<stdexcept>
#include<sys/stat.h>
#include <sys/mman.h>
#include<csignal>

class MutableMemoryMappedFile
{
private:
    int fd;
    size_t size;
    char* data;

public:
    MutableMemoryMappedFile(const std::string& filepath)
    {
        fd = open(filepath.c_str(), O_RDONLY);
        if(fd == -1) throw std::runtime_error("Failed to open file");
        struct stat sb;
        if(fstat(fd, &sb) == -1) throw std::runtime_error("Failed to get file size");
        size = sb.st_size;

        data = static_cast<char*>(mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));
        if(data == MAP_FAILED) throw std::runtime_error("mmap failed");
    }
    ~MutableMemoryMappedFile()
    {
        if(data != MAP_FAILED) munmap(data, size);
        if(fd != -1) close(fd);
    }

    char* begin() {return data;}
    char* end() {return data + size;}
    size_t getSize() {return size;}
};

#endif //EMAIL_MEMORY_HPP
