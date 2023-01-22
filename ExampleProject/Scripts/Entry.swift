import Engine

public func Entry() {
    System("MoveSystem") {
        let transform: UnsafeMutablePointer<TransformComponent> = $1
        let movement: UnsafeMutablePointer<MoveComponent> = $2
    }
}