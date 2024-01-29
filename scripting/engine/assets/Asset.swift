import Foundation

class Asset {
    var name: String
    var path: String
    var type: String
    var data: Data?
    var uuid: String
    
    init(name: String, path: String, type: String, data: Data, uuid: String) {
        self.name = name
        self.path = path
        self.type = type
        self.uuid = uuid
        self.data = data
    }
}