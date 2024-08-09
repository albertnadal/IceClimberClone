# Remake of "Ice Climber" (NES) game

Hi! I've been working on a first proof of concept, and it looks like everything is taking shape and gaining momentum. I'm sharing my journey in a devlog format, and the project's source code is 100% open, meaning the entire process is as transparent as possible. I encourage you to sit in the co-pilot's seat and observe the journey from a privileged point of view. I think it could be a lot of fun!

# Build instructions (macOS)

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

# Devlog #1 - Introducing the project

[![Making a Nintendo Classic: Building "Ice Climber" from Scratch - Devlog #1](https://img.youtube.com/vi/Tqkr4bJNXWg/0.jpg)](https://www.youtube.com/watch?v=Tqkr4bJNXWg)

# Devlog #2 - About the Game Loop

[![Building "Ice Climber" from Scratch - Implementing the Game Loop - Devlog #2](https://img.youtube.com/vi/hnqatUKSv_g/0.jpg)](https://www.youtube.com/watch?v=hnqatUKSv_g)

# Devlog #3 - Using the State pattern

[![Building "Ice Climber" from Scratch - Using the State Pattern - Devlog #3](https://img.youtube.com/vi/vrBrN6ftyIs/0.jpg)](https://www.youtube.com/watch?v=vrBrN6ftyIs)