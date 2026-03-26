# Suru


## Build

- Install Build Dependencies:

```
bash
sudo apt install -y build-essential cmake extra-cmake-modules qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libkf6coreaddons-dev libkf6config-dev libkf6i18n-dev libkf6kcmutils-dev libkdecorations3-dev
```

- Build the project:

```
bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
```

- Install the project:

```
bash
sudo cmake --install build
```
