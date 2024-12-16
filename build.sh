compile=true
build_dev=true
test=true

# python main.py

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

# || Computing autocorrelation ... finished in 21s
