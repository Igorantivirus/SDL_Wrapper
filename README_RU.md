<h1 align="center">SDLWrapper (SDL3 C++ Wrapper)</h1>

<p align="center">
  <b>Легковесная объектно-ориентированная обёртка над SDL3 с SFML-подобным синтаксисом.</b>
</p>

[![English version](https://img.shields.io/badge/English%20version-blue)](README.md)
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Особенности

- SFML-like синтаксис и структура.
- Чистый ООП: прямые вызовы SDL спрятаны за классами.
- Минимализм: `SDLWrapper` собирается как статическая библиотека и не собирает SDL3 за вас.
- Windows (MSVC) и Android (NDK); Linux требует проверки.

## Что реализовано

- Рендер/окно: `RenderTarget`, `RenderWindow`, `View`, `VideoMode`
- Фигуры: `Shape`, `RectangleShape`, `CircleShape`, `EllipseShape`
- Текстуры/спрайты: `Texture` (загрузка через SDL3_image), `Sprite`
- Трансформации: `Transformable`
- Вспомогательные операторы/типы: `Operators` (Rect/Point и др.), `Convert`, `Colors`

## Планируется

- `Text`
- `Image`

## Зависимости

- Язык: C++20+.
- SDL3 и SDL3_image:
  - Для сборки `SDLWrapper` нужны только заголовки (пути задаются через `SDL3_INC_DIR` и `SDL3_IMAGE_INC_DIR`).
  - Для использования `SDLWrapper` в своём приложении нужно линковаться с SDL3 и SDL3_image (и чтобы `find_package(SDL3 CONFIG)` / `find_package(SDL3_image CONFIG)` находили их).

## Сборка и установка

Ниже описан базовый алгоритм конфигурации, сборки и установки через CMake.

### 1) Клонирование

```bash
git clone https://github.com/Igorantivirus/SDLWrapper
cd SDLWrapper
```

### 2) Конфигурация

При конфигурации нужно указать пути к заголовкам SDL:
- `SDL3_INC_DIR` - путь к `.../SDL3/include`
- `SDL3_IMAGE_INC_DIR` - путь к `.../SDL3_image/include` (или к каталогу, где лежат заголовки `SDL3_image`)

#### Windows (MSVC)

```powershell
cmake -S . -B build/win -G "Ninja Multi-Config"
  -DBUILD_SHARED_LIBS=OFF
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>"
  -DSDL3_INC_DIR="D:/path/to/SDL3/include"
  -DSDL3_IMAGE_INC_DIR="D:/path/to/SDL3_image/include"
```

Если нужен рантайм/MD`, используйте:
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

### 3) Сборка

```powershell
cmake --build build/win --config Release
cmake --build build/android-arm64 --config Release
```

### 4) Инсталляция

```powershell
cmake --install build/win --config Release --prefix "/path/to/install/SDLWrapper/win/Release"
cmake --install build/android-arm64 --config Release --prefix "/path/to/install/SDLWrapper/arm64-v8a/Release"
```

После установки доступен `find_package(SDLWrapper CONFIG REQUIRED)`.

## Подключение к проекту (CMake)

```cmake
# 1) Путь к установленному SDLWrapper
list(APPEND CMAKE_PREFIX_PATH "/path/to/install/SDLWrapper/win/Release")

# 2) SDL3 и SDL3_image (у вас должны быть установлены с CMake config-файлами)
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

## Поддержка платформ

- Windows: MSVC 2022, поддерживаются /MD и /MT.
- Android: протестировано с NDK 26.1.10909125, API 24.
- Linux: требуется тестирование.

## Лицензия

MIT License.
