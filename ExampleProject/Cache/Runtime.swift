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


    let playerMeshIds = AssetDatabase.shared.loadModel(at: "models/spaceship/scene.gltf")

    let playerMatId = AssetDatabase.shared.createNewMaterial(named: "Player", shaderId: shaderId)
    let playerMat = Material(internalRefId: playerMatId)
    let playerTex = AssetDatabase.shared.loadTexture(at: "models/spaceship/textures/Material.001_baseColor.png")
    let playerNor = AssetDatabase.shared.loadTexture(at: "models/spaceship/textures/Material.001_baseColor.png")
    playerMat.setTexture(named: "Diffuse", texture: playerTex)
    playerMat.setTexture(named: "Normal", texture: playerNor)

    Entity("PlayerEntity") { 
        TransformComponent(
            position: Vector3(x: 0, y:0, z: 10),
            rotation: Vector4(w: 0, x: 0, y: 0, z: 0),
            scale: Vector3(x: 0.5, y: 0.5, z: 0.5)
        )
        MoveComponent(
            movement: Vector3(x: 0, y: 0, z: 0)
        )
        MaterialComponent(material: playerMat)
        MeshComponent(mesh: Mesh(internalRefId: playerMeshIds.first!))
        PlayerComponent(id: 0)
    }

    let asteroidMeshIds = AssetDatabase.shared.loadModel(at: "models/asteroid/scene.gltf")
    let asteroidMatId = AssetDatabase.shared.createNewMaterial(named: "Test", shaderId: shaderId)
    let asteroidMat = Material(internalRefId: asteroidMatId)
    let asteroidTex = AssetDatabase.shared.loadTexture(at: "models/asteroid/textures/Standard_baseColor.jpeg")
    let asteroidNor = AssetDatabase.shared.loadTexture(at: "models/asteroid/textures/Standard_normal.png")
    asteroidMat.setTexture(named: "Diffuse", texture: asteroidTex)
    asteroidMat.setTexture(named: "Normal", texture: asteroidNor)


    for x in 0..<50 {
        Entity("Test\(x)") { 
            TransformComponent(
                position: Vector3(x: Float.random(in: -50..<50), y: Float.random(in: -50..<50), z:10),
                rotation: Vector4(w: 0, x: 0, y: 0, z: 0),
                scale: Vector3(x: Float.random(in: -1..<5), y: Float.random(in: -1..<5), z: Float.random(in: -1..<5))
            )
            RigidBodyComponent(
                mass: Float.random(in: 1..<50),
                isStatic: false
            )
            SphereColliderComponent(
                radius: Float.random(in: 0.1..<2)
            )
            MeshComponent(mesh: Mesh(internalRefId: asteroidMeshIds.first!))
            MaterialComponent(material: asteroidMat)
        }
    }
}

@_cdecl("update") public func update(frameTime: Float) {
    updateNative()
}