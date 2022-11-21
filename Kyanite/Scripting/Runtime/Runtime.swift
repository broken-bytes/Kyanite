import CBindings
import WinSDK

class Runtime {
    private let native: HMODULE!
    init() {
        native = LoadLibraryA("./Kyanite-Runtime.dll")
        var mesh = Mesh()
    }

    func drawMesh() {
        LoadMeshGPU()
    }
}