public class ListOutput: Output {
    public func output(structure: DataStructure) throws {
        if structure is DataTable {

        } else {
            throw OutputError.badFormat(expected: DataTable.self, actual: type(of: structure).self)
        }
    }
}