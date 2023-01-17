import WinSDK

public class Logger {
    public static let shared = Logger() 

    private init() {

    }

    public func print(str: String) {
        OutputDebugStringA(str)
    }

     public func println(str: String) {
        OutputDebugStringA("\(str)\n")
    }
}