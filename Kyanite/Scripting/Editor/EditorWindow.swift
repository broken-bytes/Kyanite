public protocol EditorWindow {
    var width: Float { get }
    var height: Float { get }
    var foreground: Bool { get }
    var title: String { get }
    
    func draw()
}