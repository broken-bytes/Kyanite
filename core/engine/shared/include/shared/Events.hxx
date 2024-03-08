#pragma once

#include "Exported.hxx"
#include <shared/Exported.hxx>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum EventType {
    System,
    Key,
    Mouse,
} typedef EventType;

enum SystemEventType {
    Quit,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowMoved,
} typedef SystemEventType;

struct WindowMovedEvent {
    int x;
    int y;
} typedef WindowMovedEvent;

struct WindowResizeEvent {
    int width;
    int height;
} typedef WindowResizeEvent;

struct WindowFocusEvent {
    bool focused;
} typedef WindowFocusEvent;

struct WindowCloseEvent {
} typedef WindowCloseEvent;

struct QuitEvent {
} typedef QuitEvent;

struct SystemEvent {
    SystemEventType type;
    union {
        WindowCloseEvent windowClose;
        WindowResizeEvent windowResize;
        WindowFocusEvent windowFocus;
        WindowMovedEvent windowMoved;
        QuitEvent quit;
    } data;
} typedef SystemEvent;

enum KeyEventType {
    KeyPressed = 1,
    KeyReleased = 0,
} typedef KeyEventType;

struct KeyEvent {
    KeyEventType type;
    uint32_t scancode;
    int mods;
} typedef KeyEvent;

enum MouseEventType {
    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled,
} typedef MouseEventType;

struct MouseEvent {
    MouseEventType type;
    int x;
    int y;
    int button;
    int mods;
} typedef MouseEvent;

struct Event {
    EventType type;
    union {
        SystemEvent system;
        KeyEvent key;
    } data;
    bool handled;
} typedef Event;