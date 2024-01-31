// This will be generated at some point, but for now it's manually created

@main
public struct Kyanite {
    public static func main() throws {
        let engine = Engine()
        engine.start()
    }
}

// Macros
@attached(member)
@attached(memberAttribute)
@attached(extension, conformances: Component, Hashable)
public macro Component() = #externalMacro(module: "Macros", type: "ComponentMacro")

@attached(member, names: arbitrary)
public macro System() = #externalMacro(module: "Macros", type: "SystemMacro")