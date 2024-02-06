import Foundation
import Native

public class Shader: Asset<NativeShader> {
    required override internal init(uuid: String, native: NativeShader) {
        super.init(uuid: uuid, native: native)
    }
}