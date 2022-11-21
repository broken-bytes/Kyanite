import Foundation

public struct Timing {
    public static func measure(block: () -> Void) -> Float {
        let start = Date.now
        block()
        return Float(Calendar.current.dateComponents([.nanosecond], from: start, to: Date.now).nanosecond!) / 1000000.0
    }
}