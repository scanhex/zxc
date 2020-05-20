# zxc
```
git clone https://github.com/scanhex/zxc
cd zxc
```

## Подмодули -- Magnum 2019.10
```
git submodule update --init
```

Инструкции по установке библиотек:

## Ubuntu
Boost 1.65+
```
apt-get install libboost-system-dev
apt-get install libboost-thread-dev
apt-get install libboost-date-time-dev
apt-get install libboost-regex-dev
apt-get install libboost-serialization-dev
```
или целиком
```
apt-get install libboost-all-dev   
```

OpenGL & SDL2 - 2.0.10

```
apt-get install libgl1-mesa-dev
apt-get install libsdl2-dev
```

## macOS
```
brew install boost   
brew install sdl2
```

## Windows(MSVC)
Скачать SDL2 в директорию проекта с https://www.libsdl.org/release/SDL2-devel-2.0.10-VC.zip    
Скачать бинарники Boost: https://sourceforge.net/projects/boost/files/boost-binaries/   
Также нужно установить переменные среды `BOOST_INCLUDEDIR` на папку вида `boost_1_65_0` и `BOOST_LIBRARYDIR` на папку `boost_1_65_0\lib64-msvc-14.1`

## Windows(MinGW )
Аналогично MSVC, но бинарники Boost скачать отсюда не получится.
SDL2 можно скачать с того же сайта, но файл будет с именем `SDL2-devel-2.0.10-mingw.tar.gz`

## Запуск
Просто запустить цель `zxc` в CMake.


