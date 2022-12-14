#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class PrintWorkingDirectory: Command {
    public let keyword = "pwd"


    public func run(input: [String]) -> DataStructure {
        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        let path = getcwd(ptr, 1024)
        Console.default.print(color: .white, background: .black, str: "\(String(cString: path!))")

        return DataValue(value: path, type: .text)
    }
}