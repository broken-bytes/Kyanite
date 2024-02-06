import Foundation
import Native

public class Texture: Asset<NativeTexture> {
    required override internal init(uuid: String, native: NativeTexture) {
        super.init(uuid: uuid, native: native)
    }
}