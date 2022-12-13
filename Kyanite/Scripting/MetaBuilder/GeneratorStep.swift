//
//  File.swift
//  
//
//  Created by Kulina, Marcel on 12.09.22.
//

import Foundation

protocol GeneratorStep {
    func setUp()
    func run()
    func cleanUp()
}
