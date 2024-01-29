@propertyWrapper
struct Reference<T> {
    var uuid: String?

    init(uuid: String? = nil) {
        self.uuid = uuid
    }

    var wrappedValue: T? {
        get {
            guard let uuid = uuid else {
                return nil
            }
            
            
        }
    }
}