```
brew install glfw3
$ vim ~/.zshrc
> export CPATH=/opt/homebrew/include
> export LIBRARY_PATH=/opt/homebrew/lib
```

```
wget https://github.com/raysan5/raylib/archive/master.zip
unzip master.zip
cd raylib-master/src
make PLATFORM=PLATFORM_DESKTOP
cp libraylib.a ../../IceClimberClone/third_party/raylib
```

```
make all
./main
```