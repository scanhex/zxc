# zxc
```
git clone https://github.com/scanhex/zxc
cd zxc
```

## Submodules
```
git submodule update --init
```

## Ubuntu
```
sudo apt-get install libboost-system-dev
sudo apt-get install libboost-thread-dev
sudo apt-get install libboost-date-time-dev
sudo apt-get install libboost-regex-dev
sudo apt-get install libboost-serialization-dev
```
или целиком
```
sudo apt-get install libboost-all-dev   
```


```
sudo apt-get install libgl1-mesa-dev
sudo apt-get install libsdl2-dev
```

## macOS
```
brew install boost   
brew install sdl2
```

## Windows(MSVC)
Download SDL2 to the project directory from https://www.libsdl.org/release/SDL2-devel-2.0.10-VC.zip    
Download Boost binaries: https://sourceforge.net/projects/boost/files/boost-binaries/   
You also need to make `BOOST_INCLUDEDIR` environment variable point to the folder that looks like `boost_1_65_0` and make `BOOST_LIBRARYDIR` to point to the folder like `boost_1_65_0\lib64-msvc-14.1`

## Windows(MinGW)
Similar to MSVC, but you can't download Boost binaries from the same website.
You can download SDL2 from the same place, but the filename will be `SDL2-devel-2.0.10-mingw.tar.gz`

## Run
Just run `zxc` CMake target.

## Libraries used 
* Magnum 2019.10
* Corrade 2019.10
* Boost 1.65+
* OpenGL & SDL2 - 2.0.10
* Dear ImGui 1.76
* stb_truetype.h 1.24
* Assimp 5.01
