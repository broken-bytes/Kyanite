//
//  File.swift
//  
//
//  Created by Kulina, Marcel on 12.09.22.
//

@main
struct Builder {
    static func main() {
        generate()
    }

    public static func generate() {
        let steps: [GeneratorStep] = [JobBindingsStep(), MetaDataStep(), ComponentBindingStep(), RuntimeStep()]
        for step in steps {
            step.setUp()
            step.run()
        }
    }
}
