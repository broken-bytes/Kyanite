// This will be generated at some point, but for now it's manually created

import Macros

@_cdecl("kyanitemain")
public func kyanitemain() {
    let engine = Engine()
    engine.start()
}

// Macros
@attached(member, names: named(init))
@attached(memberAttribute)
@attached(extension, conformances: Hashable)
internal macro Component() = #externalMacro(module: "Macros", type: "ComponentMacro")

@attached(member, names: arbitrary)
internal macro System() = #externalMacro(module: "Macros", type: "SystemMacro")
