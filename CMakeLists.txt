cmake_minimum_required(VERSION 3.22)
project(parkerwords)

#gcc++
#set(CMAKE_C_COMPILER /home/draghan/tools/gcc-master-bin/bin/gcc)
#set(CMAKE_CXX_COMPILER /home/draghan/tools/gcc-master-bin/bin/g++)
#add_link_options(-L/home/draghan/tools/gcc-master-bin/lib64 -Wl,-rpath,/home/draghan/tools/gcc-master-bin/lib64)
set(CMAKE_C_COMPILER gcc-12)
set(CMAKE_CXX_COMPILER g++-12)
add_compile_options(-std=c++2b -fconstexpr-ops-limit=999999999999999 -fconstexpr-loop-limit=2147483647 -O3)
#gcc--

##clang++
#set(CMAKE_C_COMPILER /home/draghan/tools/llvm-project/build/bin/clang)
#set(CMAKE_CXX_COMPILER /home/draghan/tools/llvm-project/build/bin/clang++)
#add_compile_options(-std=c++2b -nostdinc++ -I/home/draghan/tools/llvm-project/build/include/c++/v1 -std=c++2b -fconstexpr-steps=4294048576)
#add_link_options(-L/home/draghan/tools/llvm-project/build/lib -lc++ -nostdinc++ -Wl,-rpath,/home/draghan/tools/llvm-project/build/lib -Wl,-rpath,/home/draghan/tools/llvm-project/build/lib/x86_64-unknown-linux-gnu/)
##clang--

add_executable(original non_intersecting_words.cpp)
add_executable(constexpr constexpr.cpp)

# generate preprocessed words_alpha.txt to include as a string literal to the C++ code:
file(READ words_alpha.txt original_content)
set(escaped_content "R\"(\n${original_content})\"")
file(WRITE words_alpha.generated.inc "${escaped_content}")
