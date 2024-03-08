// This will be generated at some point, but for now it's manually created

import Macros

@_cdecl("kyanitemain")
public func kyanitemain(window: UnsafeMutableRawPointer, isDebug: Bool = false) {
    let engine = Engine(window: window, isDebug: isDebug)
    engine.start()
}

// Macros
@attached(member, names: named(init))
@attached(memberAttribute)
@attached(extension, conformances: Component, Hashable, Equatable)
public macro Component() = #externalMacro(module: "Macros", type: "ComponentMacro")

@attached(member, names: arbitrary)
public macro System() = #externalMacro(module: "Macros", type: "SystemMacro")
