import Native

public class Model: Asset<NativeModel> {
    public var meshes: [Mesh] = []

    override internal init(uuid: String, native: NativeModel) {
        super.init(uuid: uuid, native: native)
    }
}