#if os(Windows)
import WinSDK
#endif

import Foundation

class List: Command {
    public let keyword = "ls"


    public func run(input: [String]) -> Bool {
        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        let pathPtr = getcwd(ptr, 1024)
        let fm = FileManager.default
        let path = Bundle.main.resourcePath!

        do {
            let items = try fm.contentsOfDirectory(atPath: path)

            for item in items {
                 print("Found \(item)")
            }
        } catch {
    // failed to read directory – bad permissions, perhaps?
        }

        return true
    }
}