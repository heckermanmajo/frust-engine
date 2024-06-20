# add this to the cmake command: cmake -DCMAKE_BUILD_TYPE=Release ...
mkdir ./cmake-build-debug
cd ./cmake-build-debug &&\
cmake  -G Ninja .. &&\
ninja &&\
cp ./mages ../mages &&\
cd ../ &&\
./mages

