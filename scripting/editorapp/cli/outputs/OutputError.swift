public enum OutputError: Error {
    case badFormat(expected: DataStructure.Type, actual: DataStructure.Type)
    case unknownFormat(format: DataStructure.Type)
    case unknownError
}