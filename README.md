# Suru

- **Suru** is loosley inspired by Ubuntu's [Yaru](https://github.com/ubuntu/yaru) and [Yaru.dart](https://github.com/ubuntu/yaru.dart) project but for KDE Plamsa Desktop instead.
- Uses KDE Plamsa's Fantastic [Breeze](https://github.com/kde/breeze) as it's upstream base (_kinda like fork of Breeze_)

 ## Screenshots
- Kdecoration

  | breeze | suru |
  | ------ | -----|
  | <img width="1556" height="1480" alt="breeze-decoration" src="https://github.com/user-attachments/assets/fc74207b-3742-484e-b9af-41f3827e7127" /> | <img width="1556" height="1480" alt="suru-kdecoration" src="https://github.com/user-attachments/assets/ead72e90-3bf0-4e39-a4b6-1cde175230e3" /> |


## Build

- Install Build Dependencies:

```bash
sudo apt install -y build-essential cmake extra-cmake-modules qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6dbus6 libkf6coreaddons-dev libkdecorations3-dev
```

- Build the project:

```bash
cmake -S . -B build
cmake --build build
```

- Install the project:

```bash
sudo cmake --install build
```


> [!IMPORTANT]
> GitHub's Copilot (via VS-Code) has been used to add the comments in the project, just exploring Agentic approaches for docs and linting
