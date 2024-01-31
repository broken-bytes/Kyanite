@System class RenderSystem {
    static func run(meshes: UnsafeMutableBufferPointer<MeshComponent>, transforms: UnsafeMutableBufferPointer<TransformComponent>) {
        // COmbine the mesh and transform components into a single buffer and iterate over it and change the position of the transform
       for x in 0..<transforms.count {
           transforms[x].position.x += 0.01
       }
    }
}
