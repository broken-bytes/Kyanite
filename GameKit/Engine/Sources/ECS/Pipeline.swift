import Foundation

public extension ECS {

    class Pipeline {

        public static let onUpdate: Pipeline = Pipeline(with: "OnUpdate", ticking: 0)
        
        // The descriptive name of this pipeline
        let name: String
        // The Tickrate of this pipeline in ms. 1 = Once per Second = 1 FPS, 1000 = 1000x per second
        var tickRate: UInt16
        // The systems attached to this pipeline
        var systems: [System] = []

        init(with name: String, ticking at: UInt16) {
            self.name = name
            self.tickRate = at
        }

        public func register(system: System) {
            //systems.append(system)
        }

        // Ticks the pipeline(Advances it) and returns the time it took for the pipeline to run in ms.
        internal func tick() -> UInt64 {
            let tick = ContinuousClock().measure {
                for system in systems {
                    system.function(SystemIterator(for: system.archetype))
                }
            }

            print("Pipeline tick: \(tick)")

            return 0
        }
    }
}