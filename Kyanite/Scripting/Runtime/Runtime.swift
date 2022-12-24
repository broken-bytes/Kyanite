import WinSDK
import Foundation

class Runtime {
    private static let `default` = Runtime()

    private var engine: HMODULE! 

    private init() {
        self.engine = LoadLibraryA("Kyanite.dll")
    }

    public func loadFunc<T>(named: String) -> UnsafeMutablePointer<T> {
        var ptr = GetProcAddress(self.engine, named)

        let rawPtr = UnsafeMutableRawPointer(mutating: ptr)

        return rawPtr.load(as: T.self)
    }
}