#!/bin/sh

PROJECT_NAME=$(find . -name "*.xcodeproj" | sed 's/\.xcodeproj//g' | sed 's/^\.\///g' | tr '[:upper:]' '[:lower:]')
VERSION=$(grep "#define VERSION " src/include/config.h | cut -d " " -f 3 | tr -d '"')
echo "Building $PROJECT_NAME.xpl version $VERSION. Is this correct? (y/n):"
read CONFIRM

if [ "$CONFIRM" != "y" ]; then
    echo "Please update the version number in config.h and try again."
    exit 1
fi

AVAILABLE_PLATFORMS="mac win lin"
echo "Which platforms would you like to build? ($AVAILABLE_PLATFORMS):"
read PLATFORMS

if [ -z "$PLATFORMS" ]; then
    PLATFORMS=$AVAILABLE_PLATFORMS
fi

for platform in $PLATFORMS; do
    if ! echo $AVAILABLE_PLATFORMS | grep -q $platform; then
        echo "Invalid platform: $platform. Exiting."
        exit 1
    fi
done

echo "Building for platforms: \033[1m$PLATFORMS\033[0m\n"
echo "Clean build directory? (y/n):"
read CLEAN_BUILD

if [ "$CLEAN_BUILD" = "y" ]; then
    echo "Cleaning build directories..."
    if [ -d "build" ]; then
        rm -rf build
    fi
fi

for platform in $PLATFORMS; do
    echo "Building $platform..."
    cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-$platform.cmake -Bbuild/$platform -H.
    make -C build/$platform
    if [ $? -eq 0 ]; then
        echo "\n\n"
        echo "\033[1;32m$platform build succeeded.\033[0m\nProduct: build/$platform/${platform}_x64/${PROJECT_NAME}.xpl"
        file build/$platform/${platform}_x64/${PROJECT_NAME}.xpl
        sleep 3
    else
        echo "\033[1;31m$platform build failed.\033[0m"
        exit 1
    fi
done

echo "Building done."

echo "Creating distribution bundle..."
if [ -d "build/dist" ]; then
    rm -rf build/dist
fi

for platform in $AVAILABLE_PLATFORMS; do
    mkdir -p build/dist/${platform}_x64
    if [ -d "build/$platform/${platform}_x64" ]; then
        cp build/$platform/${platform}_x64/${PROJECT_NAME}.xpl build/dist/${platform}_x64/${PROJECT_NAME}.xpl
    fi

    if [ -d "lib/${platform}_x64/dist" ]; then
        cp -r lib/${platform}_x64/dist/* build/dist/${platform}_x64
    fi
done

cp -r assets build/dist
mkdir -p build/dist/cache
default_ini=$(sed -n '/const char \*defaultConfig = R"(/,/^)";/p' "./src/include/appstate.cpp" | sed '$d' | sed '1s/const char \*defaultConfig = R"(//' | sed 's/^[ \t]*//')
echo "$default_ini" > build/dist/config.ini

cd build
mv dist $PROJECT_NAME
zip -rq $PROJECT_NAME-$VERSION.zip $PROJECT_NAME
mv $PROJECT_NAME dist
cd ..

echo "Bundle created. Distribution: build/dist/$PROJECT_NAME-$VERSION.zip"
