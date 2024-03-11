@_implementationOnly import Native

public class HGroup {
    public init(_ block: @escaping () -> Void) {
        NativeImGui.shared.beginHGroup()
        block()
        NativeImGui.shared.endHGroup()
    }
}