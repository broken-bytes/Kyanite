class Engine {
    private var time: Float = 0 
    private var window: NativePointer? = nil
    
    init() {
        // Initialize all subsystems
        // Initialize the core
        Core_Init()
        // Initialize the window
        window = Core_CreateWindow("Test", nil, nil, 800, 600, 0, 0, false)
        Audio_Init()
        Input_Init()
        Rendering_Init(window)
    }

    func start() {
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                Renderer.shared.preFrame()
                InputManager.shared.update()
                ECS_Update(time)
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