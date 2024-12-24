compile=true
build_dev=true
test=true
oflags=false

if ${compile}; then

    mkdir -p build
    cd build

    if ${build_dev}; then
        cmake \
            -D CMAKE_BUILD_TYPE=DEBUG \
            -D BUILD_TESTING=TRUE \
            -D CMAKE_CXX_COMPILER="/opt/homebrew/bin/g++-14" \
            ..
            # -D PARALLEL=TRUE \
    elif ${oflags}; then
        cmake -D CMAKE_CXX_COMPILER="/opt/homebrew/bin/g++-14" \
              -D OPT=TRUE \
              ..
    else
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
