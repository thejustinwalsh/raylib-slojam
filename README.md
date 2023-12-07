# raylib-slojam
> https://itch.io/jam/raylib-slo-jam

## Dependencies

```
brew install emscripten cmake ninja
```

## Build

To build this project, make sure to have CMake installed locally.

### Desktop

```
mkdir build
cmake -B ./build
cmake --build ./build
```

### Web

```
mkdir build
emcmake cmake -B ./build-web -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release
cmake --build ./build-web
```

## Run

```sh
./build/raylib_gamejam_template
- or -
emrun ./build-web/raylib_gamejam_template.html
```
