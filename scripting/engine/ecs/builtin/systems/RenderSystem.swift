@System class RenderSystem {
    func run(mesh: UnsafeBufferPointer<MeshComponent>, transform: UnsafeBufferPointer<TransformComponent>) {
        print("Running render system")
        for (mesh, transform) in zip(mesh, transform) {
            print("Rendering mesh \(mesh.mesh) with transform \(transform)")
        }
    }
}