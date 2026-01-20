#include <SDLWrapper/FileWorker.hpp>

static void fillMode(char modeStr[4], const sdl3::FileWorkerMode fmode)
{
    std::memset(modeStr, 0, 4);
    unsigned char ind = 0;
    modeStr[ind++] =
        static_cast<bool>(fmode & sdl3::FileWorkerMode::read) ? 'r' : static_cast<bool>(fmode & sdl3::FileWorkerMode::write) ? 'w'
                                                                  : static_cast<bool>(fmode & sdl3::FileWorkerMode::app)     ? 'a'
                                                                                                                             : '\0';
    if (static_cast<bool>(fmode & sdl3::FileWorkerMode::binary))
        modeStr[ind++] = 'b';
    if (static_cast<bool>(fmode & sdl3::FileWorkerMode::dblmode))
        modeStr[ind++] = '+';
}

namespace sdl3
{
FileWorker::FileWorker(const std::string_view path, const FileWorkerMode mode)
{
    open(path, mode);
}
FileWorker::FileWorker(const std::filesystem::path &path, const FileWorkerMode mode)
{
    open(path, mode);
}
FileWorker::~FileWorker()
{
    close();
}

bool FileWorker::open(const std::filesystem::path &path, const FileWorkerMode mode)
{
    std::string stringPath = path.string();
    std::string_view view = stringPath;
    return open(view, mode);
}
bool FileWorker::open(const std::string_view path, const FileWorkerMode mode)
{
    char strMode[4] = {};
    fillMode(strMode, mode);
    io_ = SDL_IOFromFile(path.data(), strMode);
    mode_ = mode;
    return static_cast<bool>(io_);
}

bool FileWorker::close()
{
    mode_ = static_cast<FileWorkerMode>(0);
    bool res = false;
    if (io_)
        res = SDL_CloseIO(io_);
    io_ = nullptr;
    return false;
}

bool FileWorker::isOpen() const
{
    return static_cast<bool>(io_);
}

std::string FileWorker::readAll()
{
    const auto sz = SDL_GetIOSize(io_);
    std::string buf(static_cast<size_t>(sz), '\0');
    SDL_ReadIO(io_, buf.data(), buf.size());
    return buf;
}
std::string FileWorker::readNext(const Sint64 size)
{
    std::string buf(static_cast<size_t>(size), '\0');
    SDL_ReadIO(io_, buf.data(), buf.size());
    return buf;
}
bool FileWorker::readNext(char* str, const Sint64 size)
{
    return SDL_ReadIO(io_, str, size);
}

bool FileWorker::write(const std::string_view str)
{
    size_t bytesWritten = SDL_WriteIO(io_, str.data(), str.size());
    return bytesWritten == str.size();
}

bool FileWorker::seek(long offset, const SDL_IOWhence type)
{
    Sint64 result = SDL_SeekIO(io_, offset, type);
    return result != -1;
}
size_t FileWorker::tell()
{
    Sint64 position = SDL_TellIO(io_);
    if (position == -1)
        return 0;
    return static_cast<size_t>(position);
}
} // namespace sdl3