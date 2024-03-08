import Native

public class Float4Field: InputField {
    var buffer: UnsafeMutableBufferPointer<Float>
    var label: String

    public init(label: String, buffer: UnsafeMutablePointer<Float>) {
        self.label = label
        self.buffer = UnsafeMutableBufferPointer(start: buffer, count: 4)
    }

    public override func draw() {
        guard let base = buffer.baseAddress else {
            return
        }

        NativeImGui.shared.float4Field(label, value: base)
    }
}