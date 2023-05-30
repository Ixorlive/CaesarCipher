project(CaesarCipher)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

SET(SOURCE src/main.cpp
        src/caesar_cipher.cpp
        src/crack_cipher_system.cpp
        src/user_interaction.cpp
        src/string_processing.cpp)

add_executable(CaesarCipher ${SOURCE})