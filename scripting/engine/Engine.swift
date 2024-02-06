import Native

class Engine {
    private var time: Float = 0 
    private var window: NativeWindow? = nil
    
    init() {
        // Initialize all subsystems
        // Initialize the core
        NativeCore.shared.start()
        // Initialize the window
        var posX: UInt32 = 0
        var posY: UInt32 = 0
        window = NativeCore.shared.createWindow(
            "Game", 
            posX: &posX, 
            posY: &posY, 
            width: 800, 
            height: 600, 
            flags: 0, 
            backend: 0, 
            silent: false
        )
        NativeAudio.shared.start()
        NativeInput.shared.start()
        guard let window = window else {
            fatalError("Failed to create window")
        }
        NativeRendering.shared.start(window: window)
    }

    func start() {
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                Renderer.shared.preFrame()
                InputManager.shared.update()
                NativeECS.shared.update(deltaTime: time)
                // Update the rendering system
                Renderer.shared.update(with: time)
                Renderer.shared.postFrame()
                // Update the swift internal systems
            }
            // Attoseconds to milliseconds
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}