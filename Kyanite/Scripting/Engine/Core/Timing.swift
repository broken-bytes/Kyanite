public class Timing {
    public static let shared = Timing()

    public internal(set) var deltaTime: Float = 0

    internal init() {

    }
}