<h1 align="center">SDLWrapper (SDL3 C++ Wrapper)</h1>

<p align="center">
  <b>A lightweight object-oriented wrapper over SDL3 with an SFML-like API.</b>
</p>

[![Russian version](https://img.shields.io/badge/Russian%20version-blue)](README_RU.md)
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Features

- SFML-like API and structure.
- OOP-first: SDL calls are hidden behind classes.
- Minimal: `SDLWrapper` builds as a static library and does not build SDL3 for you.
- Windows (MSVC) and Android (NDK); Linux needs verification.

## Implemented

- Rendering/window: `RenderTarget`, `RenderWindow`, `View`, `VideoMode`
- Shapes: `Shape`, `RectangleShape`, `CircleShape`, `EllipseShape`
- Textures/sprites: `Texture` (loaded via SDL3_image), `Sprite`
- Transforms: `Transformable`
- Helper operators/types: `Operators` (Rect/Point etc.), `Convert`, `Colors`

## Planned

- `Text`
- `Image`

## Dependencies

- Language: C++20+.
- SDL3 + SDL3_image:
  - Building `SDLWrapper` requires headers only (provided via `SDL3_INC_DIR` and `SDL3_IMAGE_INC_DIR`).
  - Using `SDLWrapper` in your app still requires linking against SDL3 and SDL3_image (and having `find_package(SDL3 CONFIG)` / `find_package(SDL3_image CONFIG)` working).

## Build and install

Below is a simple CMake workflow for configure, build, and install.

### 1) Clone

```bash
git clone https://github.com/Igorantivirus/SDLWrapper
cd SDLWrapper
```

### 2) Configure

You must provide SDL header paths:
- `SDL3_INC_DIR` - path to `.../SDL3/include`
- `SDL3_IMAGE_INC_DIR` - path to `.../SDL3_image/include` (or wherever `SDL3_image` headers are located)

#### Windows (MSVC)

```powershell
cmake -S . -B build/win -G "Ninja Multi-Config"
  -DBUILD_SHARED_LIBS=OFF
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>"
  -DSDL3_INC_DIR="D:/path/to/SDL3/include"
  -DSDL3_IMAGE_INC_DIR="D:/path/to/SDL3_image/include"
```

If you want/MD`, use:
```powershell
cmake -S . -B build/win -G "Ninja Multi-Config"
  -DBUILD_SHARED_LIBS=OFF
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:DebugDLL>"
  -DSDL3_INC_DIR="D:/path/to/SDL3/include"
  -DSDL3_IMAGE_INC_DIR="D:/path/to/SDL3_image/include"
```

#### Android (NDK)

```powershell
cmake -S . -B build/android-arm64 -G "Ninja Multi-Config"
  -DCMAKE_TOOLCHAIN_FILE=".../Android/Sdk/ndk/26.1.10909125/build/cmake/android.toolchain.cmake"
  -DANDROID_NDK=".../Android/Sdk/ndk/26.1.10909125"
  -DANDROID_PLATFORM="android-24"
  -DANDROID_ABI="arm64-v8a"
  -DBUILD_SHARED_LIBS=OFF
  -DSDL3_INC_DIR="D:/path/to/SDL3/include"
  -DSDL3_IMAGE_INC_DIR="D:/path/to/SDL3_image/include"
```

Repeat the Android configure/build/install for each ABI:
`arm64-v8a`, `armeabi-v7a`, `x86`, `x86_64` (use separate build dirs like `build/android-<abi>`).

On Windows you can also build+install all Android ABIs at once via `build_android_all.bat`
(or the VS Code task `Install: Android (all ABIs)`).

### 3) Build

```powershell
cmake --build build/win --config Release
cmake --build build/android-arm64 --config Release
```

### 4) Install

```powershell
cmake --install build/win --config Release --prefix "D:/path/to/install/SDLWrapper/win/Release"
cmake --install build/android-arm64 --config Release --prefix "D:/path/to/install/SDLWrapper/arm64-v8a/Release"
```
After installation, `find_package(SDLWrapper CONFIG REQUIRED)` is available.

## Using in your project (CMake)

```cmake
# 1) SDLWrapper install prefix
list(APPEND CMAKE_PREFIX_PATH "D:/path/to/install/SDLWrapper/win/Release")

# 2) SDL3 and SDL3_image (must be installed with their CMake config files)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

# 3) SDLWrapper
find_package(SDLWrapper CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDLWrapper::SDLWrapper
  SDL3::SDL3-static
  SDL3_image::SDL3_image-static
)
```

## Platform support

- Windows: MSVC 2022, both /MD and /MT.
- Android: tested with NDK 26.1.10909125, API 24.
- Linux: needs verification.

## License

MIT License.
