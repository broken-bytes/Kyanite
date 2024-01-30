public class Object {
    internal var refCount: Int = 0
    internal var handle: UnsafeMutableRawPointer? = nil

    init(handle: UnsafeMutableRawPointer) {
        self.handle = handle
        self.refCount = 1
    }

    func retain() {
        self.refCount += 1
    }

    func release() {
        self.refCount -= 1

        if self.refCount == 0 {
            // Clear the resource on the native side
            self.handle = nil
        }
    }
}