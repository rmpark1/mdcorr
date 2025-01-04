compile=true
dev=true
oflags=false
test=true

if ${compile}; then

    if ! [ -d "build" ]; then
        mkdir build
    else
        rm build/CmakeCache.txt
    fi 

    cd build

    if ${dev}; then
        cmake \
            -D CMAKE_BUILD_TYPE=DEBUG \
            -D BUILD_TESTING=TRUE \
            -D DEV=TRUE \
            ..
    elif ${oflags}; then
        cmake -D OPT=TRUE ..
    fi

    if [[ $OSTYPE != "linux-gnu"* ]]; then
        cmake -D CMAKE_CXX_COMPILER="/opt/homebrew/bin/g++-14" ..
    fi


    cmake --build . -j 10 
    cd ..

fi 

if ${test}; then
    cd build
    ctest --rerun-failed --output-on-failure
    cd ..
fi
