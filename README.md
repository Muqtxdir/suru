# Suru
- Kdecoration
  
  | breeze | suru |
  | ------ | -----|
  | <img width="1556" height="1468" alt="breeze-welcome" src="https://github.com/user-attachments/assets/c1606a38-e6f7-49bc-b0a2-750f9050718d" /> | <img width="1488" height="1400" alt="suru-welcome" src="https://github.com/user-attachments/assets/bd8d925a-fea3-4a1f-96bd-8fadd4bc05f4" /> |



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
