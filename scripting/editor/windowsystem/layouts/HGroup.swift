import EditorNative

public class HGroup {
    public init(_ block: @escaping () -> Void) {
        EditorNativeCore.shared.beginHGroup()
        block()
        EditorNativeCore.shared.endHGroup()
    }
}