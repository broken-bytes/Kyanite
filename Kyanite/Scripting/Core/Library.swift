import WinSDK
import Foundation

public class Library {
    private var lib: HMODULE! 

    private init(at path: String) {
        self.lib = LoadLibraryA(path)
    }

    public func loadFunc<T>(named: String) -> T {
        guard let ptr = GetProcAddress(self.lib, named) else { fatalError("Failed to load dll func \(named)")}

        return withUnsafePointer(to: ptr) { 
            let rawPtr = UnsafeMutableRawPointer(mutating: $0)
            return rawPtr.load(as: T.self)
        }
    }

    public static func loadLibrary(at path: String) -> Library {
        Library(at: path)
    }
}