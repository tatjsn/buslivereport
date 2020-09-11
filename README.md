# buslivereport

## How to build
```sh
python -m venv .venv
source .venv/bin/activate
pip install conan

cd build
conan install ..
cmake ..
cmake --build .
```

## How to run
```sh
cd build
./bin/app livereport
```
