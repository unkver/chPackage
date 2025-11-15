# "ch" as in "🧀"
### When you're too scared to install a package using your terminal
<img src="https://i.ibb.co/N61mL9jW/Screenshot-20251115-210801.png">

## Install
Make sure you have g++ and gtkmm-3.0 (libgtkmm-3.0-dev for debian-based) installed.
### Note
In wayland window managers, such as Hyprland, it refuses to start. You can temporary fix this by doing
```cpp
xhost si:localuser:root
```
right before executing chPkg with sudo.

```cpp
git clone https://github.com/unkver/chPackage
cd chPackage/
./install.sh
sudo ./chPkg
```

