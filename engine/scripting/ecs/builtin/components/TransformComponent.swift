struct TransformComponent: Component, Hashable {
    var position: [Float] = [0, 0, 0]
    var rotation: [Float] = [0, 0, 0, 1]
    var scale: [Float] = [1, 1, 1]
}