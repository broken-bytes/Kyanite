public class System {
    let id: UInt64
    let runnable: (UnsafeMutableRawPointer) -> Void

    public init<C0>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>) -> Void) where C0: Component {
        self.id = SystemDispatcher.registerSystem(name: name, multiThreaded: true, C0.self)

        self.runnable = {
            let iter: UnsafeMutableRawPointer = $0
            let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
            SystemDispatcher.getSystemDelta(iter, delta)
            let data = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C0.self, index: 1)
            
            for x in 0..<data.count {
                block(delta.pointee, data.baseAddress!.advanced(by: x))
            }
        }

        register()
    }

    public init<C0, C1>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>) -> Void) where C0: Component, C1: Component {
        self.id = SystemDispatcher.registerSystem(name: name, multiThreaded: true, C0.self, C1.self)
        
        self.runnable = {
            let iter: UnsafeMutableRawPointer = $0
            let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
            SystemDispatcher.getSystemDelta(iter, delta)
            let data1 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C0.self, index: 1)
            let data2 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C1.self, index: 2)
            
            for x in 0..<data1.count {
                block(delta.pointee, data1.baseAddress!.advanced(by: x), data2.baseAddress!.advanced(by: x))
            }
        }

        register()
    }

    public init<C0, C1, C2>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>) -> Void) where C0: Component, C1: Component, C2: Component {
        self.id = SystemDispatcher.registerSystem(name: name, multiThreaded: true, C0.self, C1.self)
        self.runnable = {
            let iter: UnsafeMutableRawPointer = $0
            let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
            SystemDispatcher.getSystemDelta(iter, delta)
            let data1 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C0.self, index: 1)
            let data2 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C1.self, index: 2)
            let data3 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C2.self, index: 3)
            
            for x in 0..<data1.count {
                block(delta.pointee, data1.baseAddress!.advanced(by: x), data2.baseAddress!.advanced(by: x), data3.baseAddress!.advanced(by: x))
            }
        }
        register()
    }

    public init<C0, C1, C2, C3>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>) -> Void) where C0: Component, C1: Component, C2: Component, C3: Component {
        self.id = SystemDispatcher.registerSystem(name: name, multiThreaded: true, C0.self, C1.self)
        self.runnable = {
            let iter: UnsafeMutableRawPointer = $0
            let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
            SystemDispatcher.getSystemDelta(iter, delta)
            let data1 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C0.self, index: 1)
            let data2 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C1.self, index: 2)
            let data3 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C2.self, index: 3)
            let data4 = SystemDispatcher.getSystemComponentSet(iterator: iter, type: C3.self, index: 4)
            
            for x in 0..<data1.count {
                block(delta.pointee, data1.baseAddress!.advanced(by: x), data2.baseAddress!.advanced(by: x), data3.baseAddress!.advanced(by: x), data4.baseAddress!.advanced(by: x))
            }
        }

        register()
    }

    private func register() {
        SystemDispatcher.systemMapping.append(self)
    }
}
