public struct Scene {
    public var entities: [Entity] = []
    
    init(entities: [Entity]) {
        self.entities = entities
    }

    init(@SceneBuilder _ builder: () -> [Entity]) {
        self.entities = builder()
    }
}
