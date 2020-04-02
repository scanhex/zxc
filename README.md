# zxc
Не забыть скачать сабмодули.

Надо установить SDL2 (brew install sdl2 или sudo apt install libsdl2-dev или на Windows скачать https://www.libsdl.org/release/SDL2-devel-2.0.10-mingw.tar.gz и распаковать в папку с проектом)

Надо скачать boost (или хотя бы библиотеку boost Asio), рекомендуется версия 1.65.
sudo apt-get install libboost-all-dev или brew install boost
Для windows скачать отсюда https://www.boost.org/users/history/version_1_65_0.html

Остальное должен сделать сам Clion, нужно просто запустить цель "zxc"

На винде могут быть проблемы с assimp.dll, также если скачать самый новый boost, то у него могут быть проблемы с Cmake-ом
