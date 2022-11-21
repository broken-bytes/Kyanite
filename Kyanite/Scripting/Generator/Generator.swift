@main
struct Generator {
    static func main() {
        let rootFolder = CommandLine.arguments[1]

        print(rootFolder)

        let generator = CodeGenerator()
        generator.generate(for: rootFolder)
    }
}