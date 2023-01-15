/// Replacement class for array. Exposed the size as the first parameter so C++ world can keep the size intact when passing back to Swift

protocol ContainerType {}

public struct Container<T: Equatable> : ContainerType {
    private var size: Int = 0
    private var data: [T] = []

    public init() {
        size = data.count
    }

    public mutating func push(element: T) {
        data.append(element)
        size = data.count
    }

    public mutating func remove(at: Int) {
        data.remove(at: at)
    }

    public mutating func remove(element: T) {
        data.removeAll(where: { $0 == element})
    }
}