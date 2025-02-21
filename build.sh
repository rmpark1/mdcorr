compile=true
dev=true
oflags=false
test=true
clear=false

if ${compile}; then

    if ! [ -d "build" ]; then
        mkdir -p build
    else
        if [ -f "build/CMakeCache.txt" ] && ${clear}; then
            rm build/CmakeCache.txt
        fi
    fi 

    cd build

    if [[ $OSTYPE != "linux-gnu"* ]]; then
        cmake -D CMAKE_CXX_COMPILER="/opt/homebrew/bin/g++-14" ..
    fi

    if ${dev}; then
        cmake \
            -D CMAKE_BUILD_TYPE=DEBUG \
            -D BUILD_TESTING=TRUE \
            -D DEV=TRUE \
            ..
    fi

    if ${oflags}; then
        cmake -D OPT=TRUE ..
    fi

    cmake --build . -j 10 
    cd ..

fi 

if ${test}; then
    cd build
    ctest --rerun-failed --output-on-failure
    cd ..
fi
