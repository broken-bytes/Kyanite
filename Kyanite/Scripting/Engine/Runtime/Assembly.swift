import Core

internal typealias AssemblyStart = @convention(c) (UnsafeMutableRawPointer) -> Void

internal class Assembly {
    internal static let shared = Assembly()
    private let lib: Library

    internal init() {
        lib = Library.loadLibrary(at: "Kyanite-Assembly.dll")
    }

    internal func loadAssembly(engine: UnsafeMutableRawPointer) {
        let startFunc: AssemblyStart = self.lib.loadFunc(named: "start")
        startFunc(engine)
    }
}