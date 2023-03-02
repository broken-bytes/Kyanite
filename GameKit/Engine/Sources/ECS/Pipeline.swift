import Foundation

public extension ECS {

    class Pipeline {

        public static let onUpdate: Pipeline = Pipeline(with: "OnUpdate", ticking: 0)
        
        // The descriptive name of this pipeline
        let name: String
        // The Tickrate of this pipeline in ms. 1 = Once per Second = 1 FPS, 1000 = 1000x per second
        var tickRate: UInt16
        // The systems attached to this pipeline
        var systems: [any SystemData] = []

        init(with name: String, ticking at: UInt16) {
            self.name = name
            self.tickRate = at
        }


        // Ticks the pipeline(Advances it) and returns the time it took for the pipeline to run in ms.
        internal func tick() -> Duration {
            let tick = ContinuousClock().measure {
                for system in systems {
                    let delta = ContinuousClock().measure {
                        var pools: [ArchetypePool] = []
                        for arch in archetypes {
                            if arch.bitMask.hasAtLeast(other: system.archetype) {
                                pools.append(archetypePools[arch.bitMask]!)
                            }
                        }
                        
                        let count = pools.reduce(0, { $0 + $1.size})
                        
                        if system.multithreaded {
                            let group = DispatchGroup()
                            
                            let cores = SystemInfo.numCores()
                            // Use Cores - 2 Number of workers, or 4 fixed cores if only 4 cores are present
                            let workers =  cores > 4 ? cores - 2 : cores
                            
                            for x in 0..<workers {
                                group.enter()
                                var individualCount = Int(floor(Float(count) / Float(workers)))
                                // The last worker might need to work on one more element if count mod workers is not 0.
                                if x == workers - 1 &&  count % Int(workers) != 0 {
                                    individualCount += count - (individualCount * Int(workers))
                                }
                                
                                print(system.typedef)
                                                                
                                group.leave()
                            }
                        } else {
                            var type = system.typedef
                            var array: [type.self] = []

                            //system.function(SystemIterator(for: system.archetype, count: count))
                        }
                    }
                    if delta > .milliseconds(1) {
                        print("||>- 🔴 System \(system.name) took \(delta)")
                    } else if delta > .milliseconds(0.5) {
                        print("||>- 🟠 System \(system.name) took \(delta)")
                    } else {
                        print("||>- 🟢 System \(system.name) took \(delta)")
                    }
                }
            }

            return tick
        }
    }
}
