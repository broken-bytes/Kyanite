#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

public class Console {
    public static let `default` = Console()

    private var currentLine: String = ""

    private init() {
    #if os(Windows)
        var hOut = GetStdHandle(STD_OUTPUT_HANDLE)
        if hOut == INVALID_HANDLE_VALUE {
            return
        }

        var dwMode: DWORD = 0;
        if !GetConsoleMode(hOut, &dwMode) {
            return
        }

        dwMode |= UInt32(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        if !SetConsoleMode(hOut, dwMode) {
            return
        }
    #endif
        var esc = UnicodeScalar(27)!
        var term = UnicodeScalar(0x07)
        Swift.print("\(esc)]0;Kyanite\(term)")
        Swift.print("\(esc)[1SPq")
    }

    public func print(color: Color, background: BackgroundColor, str: String) {
        var esc = UnicodeScalar(27)!
        Swift.print("\(esc)[\(color.rawValue)m\(str)", terminator: "")
    }

    public func clear() {
        var esc = UnicodeScalar(27)!
        Swift.print("\(esc)[2J");
        Swift.print("\(esc)[3J");
    }
}