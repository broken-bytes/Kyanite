public class ListFormatter: Formatter {
    public func print(list: DataStructure) {
        // We only want to show headers when the structure received is an actual list
        let showHeaders = list is DataTable
    }
}