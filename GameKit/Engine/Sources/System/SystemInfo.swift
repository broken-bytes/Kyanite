@_implementationOnly import HardwareInfo

public enum SystemInfo {
    
    public static func numCores() -> UInt8 {
        return HardwareInfo.numCores()
    }
}
