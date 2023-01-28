import Userland
import Engine
import Foundation
import Math
import Core

internal typealias Start = @convention(c) (UInt32, UInt32, UnsafeMutableRawPointer) -> Void
internal typealias Update = @convention(c) () -> Void
internal typealias SetRootDir = @convention(c) (UnsafePointer<Int8>) -> Void

// Pointer to the engine, Ptr to component register func

var lib: Library!
var updateNative: Update!

@_cdecl("start") public func start(
    argumentCount: UInt32,
    argumentVector: UnsafeMutablePointer<UnsafePointer<Int8>>,
    width: UInt32, 
    height: UInt32, 
    window: UnsafeMutableRawPointer
) {
    lib = Library.loadLibrary(at: "./Kyanite-Runtime.dll")
    let start: Start = lib.loadFunc(named: "Engine_Init")
    let setRootDir: SetRootDir = lib.loadFunc(named: "Engine_SetRootDir")
    updateNative = lib.loadFunc(named: "Engine_Update")
    
    start(width, height, window)

    setRootDir(argumentVector[1])

    try! ComponentRegistry.shared.register(component: TransformComponent.self)
    try! ComponentRegistry.shared.register(component: MeshComponent.self)
    try! ComponentRegistry.shared.register(component: MaterialComponent.self)
    try! ComponentRegistry.shared.register(component: MoveComponent.self)
    try! ComponentRegistry.shared.register(component: PlayerComponent.self)
    try! ComponentRegistry.shared.register(component: RigidBodyComponent.self)
    try! ComponentRegistry.shared.register(component: SphereColliderComponent.self)
    try! ComponentRegistry.shared.register(component: BoxColliderComponent.self)
    try! ComponentRegistry.shared.register(component: CapsuleColliderComponent.self)

    World("Test")


    let shaderId = AssetDatabase.shared.loadShader(at: "shaders/PBRDefault.yaml")

    var raw = UnsafeMutableRawPointer.allocate(byteCount: 1, alignment: 1)
    var count = UnsafeMutablePointer<UInt64>.allocate(capacity: 1)


    let asteroidMeshIds = AssetDatabase.shared.loadModel(at: "models/environment/floors/base_floor_stone_1.gltf")
    let asteroidMatId = AssetDatabase.shared.createNewMaterial(named: "Test", shaderId: shaderId)
    let asteroidMat = Material(internalRefId: asteroidMatId)
    let asteroidTex = AssetDatabase.shared.loadTexture(at: "models/asteroid/textures/Standard_baseColor.jpeg")
    let asteroidNor = AssetDatabase.shared.loadTexture(at: "models/asteroid/textures/Standard_normal.png")
    asteroidMat.setTexture(named: "Diffuse", texture: asteroidTex)
    asteroidMat.setTexture(named: "Normal", texture: asteroidNor)


    for x in -50..<50 {
        for y in -50..<50 {
            Entity("Test\(x):\(y)") { 
                TransformComponent(
                    position: Vector3(x: Float(x), y: -15, z: Float(y)),
                    rotation: Vector4(w: 0, x: 0, y: 0, z: 0),
                    scale: Vector3.one
                )
                MeshComponent(mesh: Mesh(internalRefId: asteroidMeshIds.first!))
                MaterialComponent(material: asteroidMat)
            }
        }
    }
}

@_cdecl("update") public func update(frameTime: Float) {
    updateNative()
}