#pragma once

#include <SDL3/SDL_log.h>
#include <string>

#include <SDL3/SDL_iostream.h>

#include <filesystem>

namespace sdl3
{

enum class FileWorkerMode : unsigned char
{
    read = 0b00000001,   // чтение
    write = 0b00000010,  // запись
    app = 0b00000100,    // добавление
    binary = 0b00001000, // бинарно
    dblmode = 0b00010000 // чтение + запись
};

inline static FileWorkerMode operator&(const FileWorkerMode left, const FileWorkerMode right)
{
    return static_cast<FileWorkerMode>(static_cast<unsigned char>(left) & static_cast<unsigned char>(right));
}

inline static FileWorkerMode operator|(const FileWorkerMode left, const FileWorkerMode right)
{
    return static_cast<FileWorkerMode>(static_cast<unsigned char>(left) | static_cast<unsigned char>(right));
}

class FileWorker
{
public:
    FileWorker() = default;
    FileWorker(const std::string_view path, const FileWorkerMode mode);
    FileWorker(const std::filesystem::path& path, const FileWorkerMode mode);
    ~FileWorker();

    bool open(const std::filesystem::path& path, const FileWorkerMode mode);
    bool open(const std::string_view path, const FileWorkerMode mode);
    bool close();

    bool isOpen() const;

    std::string readAll();
    std::string readNext(const Sint64 size);
    bool readNext(char* str, const Sint64 size);

    bool write(const std::string_view str);

    bool seek(long offset, const SDL_IOWhence type);
    size_t tell();

private:
    SDL_IOStream *io_ = nullptr;
    FileWorkerMode mode_{};
};

} // namespace sdl3