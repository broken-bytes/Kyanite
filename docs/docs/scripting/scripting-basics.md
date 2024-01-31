---
sidebar_position: 1
---

# Basics

Kyanite follows an intuitive approach for game logic creation. 

Using an ECS(Entity-Component-System), entities are easily composed of components.

## Engine Core(C++)
The Core engine is realised with multiple, mostly independent, modules. The following modules are present:

### Core
File Handling, Window Management and other low-level interactions with the system.

### Audio
Plays audio effects and makes sure the audio system is properly accessed.

### Input
Handles Keyboard, Mouse, and Controlelr Inputs. Also provides APIs to modify controller effects such as LED, Rumble, etc.

### Rendering
Provides a an abstraction layer on top of rendering APIs(currently only OpenGL).

## Scripting Layer(Swift)

The Scripting Layer is a Swift-friendly API that abstracts most lower-level and ugly syntax code away.

:::info

Due to technical limitations, some Swift APIs are available that are intended for internal use only. While these APIs can be consumed it is not advised as engine stability might get compromised. 

Those APIs are marked with underscore prefixes(`_`)

:::
