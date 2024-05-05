public class SceneManager {
    public static let `default` = SceneManager()
    var scene: (any Scene)? = nil
    var sceneNode: SceneNode? = nil

    public func loadScene(scene: some Scene) {
        self.scene = scene
        // Take the content and assign it to keep a reference to it
        sceneNode = scene.content

        scene.onDidLoad()
    }
}