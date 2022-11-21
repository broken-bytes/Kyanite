import CBindings
import WinSDK

class Runtime {
    private static let `default` = Runtime()

    private init() {
        InitCBindings()
    }
}