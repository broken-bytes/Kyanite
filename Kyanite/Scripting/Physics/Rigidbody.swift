import Math

private typealias AddRigidBodyFunc = () -> Void
public class Rigidbody {
    public var velocity: Vector3 = Vector3.zero
    public var angularVelocity: Vector3 = Vector3.zero
    public var drag: Float = 0
    public var angularDrag: Float = 0
    internal var identifier: UInt64! = nil
    internal var handle: Int! = nil
    
    public init() {
        
    }
}