# g++  -lfftw3 -lm main.cpp && ./a.out


if false; then
    rm -r build/*
    cd build
    cmake ..
    cmake --build . -j 10 
    cd ..

fi 

# ./build/mdcorr

g++ -std=c++20 temp.cpp


