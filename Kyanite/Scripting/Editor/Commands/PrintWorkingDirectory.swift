#if os(Windows)
import WinSDK
#endif

class PrintWorkingDirectory: Command {
    public let keyword = "pwd"


    public func run(input: [String]) {
        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        let path = getcwd(ptr, 1024)
        print(String(cString: path!))
    }
}