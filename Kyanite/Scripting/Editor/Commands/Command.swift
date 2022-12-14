public protocol Command {
    var keyword: String { get }

    func run(input: [String]) -> DataStructure
}