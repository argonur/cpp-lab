#!/bin/bash

# get path to script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
pushd $DIR

if [ -d "build/Release" ]; then
    echo "Delete Release build folder."
    rm -r build/Release
fi

mkdir -p build/Release

pushd ./build
echo "Generate with cmake and build:"
cmake \
-B ./Release/ \
-DCMAKE_BUILD_TYPE:STRING=Release \
..
popd > /dev/null

pushd ./build/Release

generationResult=-1

cmake --build . 

if [ $? -eq 0 ]
then
    echo "Generation finished successfully."
    generationResult=0
else
    echo "Generation failed."
    generationResult=1
fi

popd > /dev/null
popd > /dev/null

exit $generationResult
