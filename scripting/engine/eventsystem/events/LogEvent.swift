import Foundation

struct LogEvent {
    enum LogLevel: Int {
        case Debug = 0
        case Info = 1
        case Warning = 2
        case Error = 3
    }

    let message: String
    let level: LogLevel
    let category: String
    let time: Date
}