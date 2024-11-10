# g++  -lfftw3 -lm main.cpp && ./a.out


if true; then
    rm -r build/*
    cd build
    cmake ..
    cmake --build . -j 10 
    cd ..

fi 

echo "\nSTART PROGRAM\n***************\n"
./build/mdcorr

# g++ -std=c++20 temp.cpp && ./a.out


