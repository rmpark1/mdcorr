compile=true
build_dev=true
test=true
rebuild=false

if ${compile}; then
    if ${rebuild}; then rm -r build; fi

    mkdir -p build
    cd build

    if ${build_dev}; then
        cmake \
            -D CMAKE_BUILD_TYPE=DEBUG \
            -D BUILD_TESTING=DEBUG \
            ..
    else
        cmake ..
    fi

    cmake --build . -j 10 
    cd ..

fi 

if ${test}; then
    cd build
    ctest --rerun-failed --output-on-failure
    cd ..
fi
