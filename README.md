<p align="center">
  <img src="https://i.imgur.com/mF8krya.png" />
</p>

# Kyanite

The modern game engine written in C++ and Swift where coding is fun 🎉

⚠️ **ATTENTION** ⚠️

**The engine is under heavy development and most features are either missing or incomplete. Do not use this engine for any production project**

## Introduction
Kyanite is a modern lightweight game engine built around Swift. ✨

The primary focus of this project is to bring Swift into the games industry. 🎮

With a heavy focus on code first, leaving behind the usual WYSIWYG approach, Kyanite is ideal for any game that mostly deals with procedurally generated levels.

## Features
Kyanite will be a full-featured game engine on the long run.

Some of the most important features:
- 👩🏿‍💻 Scripting: Modern Swift Syntax and custom DSL to simplify many concepts 
- 🪢 Performance: Multithreaded ECS architecture for maximum performance
- 🧊 Rendering: Powerful DirectX12 3D renderer for maximum fidelity and performance
- ⚛️ Physics: Full integration with PhysX 5.1
- 🎮 Input: Coverage of almost any device thanks to latest SDL2
- 🔊 Sound: Custom platform-agnostic spatail sound system
- 📦 Assets: Modern asset pipeline including cooking, hot swapping and more
- 🔮 Futureproof core: No legacy platforms are supported 
- 🤏 Small footprint 

## Requirements
To use the engine, a Swift toolchain starting at 5.7.2 has to be installed on the Computer.

Additionally, Visual Studio Code is recommended for writing game code in Swift.

**Note:** The engine only runs on Windows 10 and never. This is a temporary restriction and macOS support is desired once the engine reaches a mature state.

## Building
Building the engine has the same requirements as using the engine plus additional things.

- A C++20 compiler(Currently only MSVC is supported, as Windows is the only target platform)
- A vcpkg installation

## Roadmap
The current roadmap expects all major features to be included in v1.0. 
Linux support is off the table for now.

|                   | Current | v1.0 | v1.1 |
|-------------------|---------|------|------|
| Rendering         | ✅       |     |     |
| Audio             | ❌       | ✅    |     |
| Animations        | ❌       | ✅    |    |
| Physics           | ❌       | ✅    |     |
| Input             | ✅       |     |     |
| macOS support     | ❌       | ❌    | ✅    |
| Linux support     | ❌       | ❌    | ❌    |
| GPU asset loading | ❌       | ❌    | ✅    |  
| Networking        | ❌       | ⏳    |     | 

Legend:
- ❌ Will not be supported in this version
- ✅ Will be introduced in this version
- ⏳ To be determined
