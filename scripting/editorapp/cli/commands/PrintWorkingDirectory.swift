#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class PrintWorkingDirectory: Command {
    public let keyword = "pwd"
    var subcommands: [Command] = []
    let shortDescription: String = "Print the current working directory"
    let longDescription: String = "Print the current working directory"

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }

        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        guard 
            let path = _getcwd(ptr, 1024),
            let str = String(cString: path, encoding: .utf8)
        else {
            return EmptyData()
        }

        ptr.deallocate()

        return DataValue(value: str, type: .text)
    }
}