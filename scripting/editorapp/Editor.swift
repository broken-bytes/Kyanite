@main
struct Editor {
    static func main() -> Void {
        let cli = CLI()
        
        return cli.run(with: CommandLine.arguments)
    }
}