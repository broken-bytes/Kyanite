public class SwiftSymbolParser {
    public static let shared = SwiftSymbolParser()

    private init() {

    }

    public func parse(source: String) -> ComponentStruct? {
        return parseComponent(source: source)
    }

    private func parseComponent(source: String) -> ComponentStruct? {
        var componentName = ""
        let groups = source.groups(for: "(public)\\s+(struct)\\s+([a-zA-Z0-9_]+)\\s*:\\s*(Component)\\s*(\\{\\X*\\})")
        if !groups.isEmpty {
            if !groups[0].isEmpty {
                componentName = groups[0][3]

                let props = parseProperties(source: groups[0][0])

                return ComponentStruct(name: componentName, properties: props)
            }
        }

        return nil
    }

    private func parseProperties(source: String) -> [Property] {
        var props: [String:String] = [:]
        let group = source.groups(for: "(public)\\s+(var)\\s+([a-zA-Z0-9_]+)\\s*:\\s*(Int|String|Bool|Float|Vector2|Vector3|Vector4)")
        for item in group {
            props[item[3]] = item[4]
        }

        return props.compactMap { Property(name: $0.key, type: $0.value) }
    }
}