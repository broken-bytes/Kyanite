#if os(Windows)
import WinSDK
#endif

class ChangeDirectory: Command {
    public let keyword = "cd"


    public func run(input: [String]) -> DataStructure {
        chdir(input[1])

        return EmptyData()
    }
}