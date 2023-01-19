//
//  File.swift
//  
//
//  Created by Kulina, Marcel on 12.09.22.
//

struct MetaBuilder {
    static func main() {
        generate()
    }

    public static func generate() {
        let steps: [GeneratorStep] = [MetaDataStep(), JobBindingsStep(), ComponentBindingStep(), RuntimeStep()]
        for step in steps {
            step.setUp()
            step.run()
        }
    }
}
