public class CLI {
    var commands: [CLICommand] = [ProjectCommand()]
    
    init() {

    }

    public func run(with args: [String]) {
        // Log the args 
        print(args)

        guard args.count > 1 else {
            fatalError("Missing command")
        }

        let command = args[1]

        for cmd in commands {
            if cmd.keyword == command {
                do {
                    let result = try cmd.execute(args: args)
                    if result != 0 {
                        fatalError("Command failed")
                    }
                } catch {
                    fatalError("Command failed")
                }
                return
            }
        }

        fatalError("Unknown command")
    }
}