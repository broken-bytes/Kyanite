//
//  File.swift
//  
//
//  Created by Kulina, Marcel on 12.09.22.
//

import Foundation

enum MetaDataError: Error {
    case componentNoMultipleInheritance(name: String)
    case componentNoClassInheritance(name: String)
    case componentUnsupportedPropertyType(name: String, property: String)
}

enum MetaDataPropertyType: String {
    case string = "String"
    case int = "Int"
    case float = "Float"
    case bool = "Bool"
    case vector2 = "Vector2"
    case vector3 = "Vector3"
    case vector4 = "Vector4"
}

struct ComponentReflectionData {
    var properties: [(String, MetaDataPropertyType)]
}

class MetaDataStep: GeneratorStep {
    func setUp() {
        
    }
    
    func run() {
        var jobs: [String] = []
        
        let path = CommandLine.arguments[1] as! String
        
        var files = [URL]()
        if let enumerator = FileManager.default.enumerator(at: URL(fileURLWithPath: path), includingPropertiesForKeys: [.isRegularFileKey], options: [.skipsHiddenFiles, .skipsPackageDescendants]) {
            for case let fileURL as URL in enumerator {
                do {
                    let fileAttributes = try fileURL.resourceValues(forKeys:[.isRegularFileKey])
                    if fileAttributes.isRegularFile! {
                        files.append(fileURL)
                    }
                } catch { print(error, fileURL) }
            }
            
            var generatedReflection = ""
            
            var components: [ComponentStruct] = []
            var jobs: [(URL, String)] = []
            
            struct MetaDataEntry: Codable {
                var file: URL
                var name: String
                
                public init(file: URL, name: String) {
                    self.file = file
                    self.name = name
                }
                
                public init(with tuple: (URL, String)) {
                    self.file = tuple.0
                    self.name = tuple.1
                }
            }
            
            struct MetaData: Codable {
                var components: [MetaDataEntry]
                var jobs: [MetaDataEntry]
            }
                        
            for file in files {
                if file.pathExtension == "swift" {
                    let data = FileManager.default.contents(atPath: file.path)
                    guard let fileData = data else { return }
                    guard let content = String(data: fileData, encoding: .utf8) else { return }
                    
                    do {
                        let parsed: ComponentStruct? = SwiftSymbolParser.shared.parse(source: content)
                        if let parsed {
                            components.append(parsed)
                        }
                    } catch {
                        print(error)
                    }
                }
            }
            
            do {
                let encoder = JSONEncoder()
                encoder.outputFormatting = .prettyPrinted
                try FileManager.default.createFile(
                    atPath: path.appending("/Cache/components.metadata"),
                    contents: encoder.encode(components)
                )
            } catch {
                print(error)
            }
        }
    }
    
    func cleanUp() {
        
    }
    
    private func checkComponentNoMultipleInheritance(symbol: Struct) throws {
        if symbol.inheritedTypes.contains (where: { (item: Class) in item.name == "Component" }) && symbol.inheritedTypes.count > 1 {
            throw MetaDataError.componentNoMultipleInheritance(name: symbol.name)
        }
    }
    
    private func checkComponentIsStruct(symbol: SwiftSymbol) throws {
        if let symbol = symbol as? Class {
            if symbol.inheritedTypes.contains (where: { (item: Class) in item.name == "Component" }) {
                throw MetaDataError.componentNoClassInheritance(name: symbol.name)
            }
        }
    }
    
    private func checkComponentPropIsBuiltinType(component: String, prop: (String, String)) throws {
        switch MetaDataPropertyType(rawValue: prop.1) {
        default:
            throw MetaDataError.componentUnsupportedPropertyType(name: component, property: prop.0)
        }
    }
    
    private func propertiesForComponent(with code: String) -> ComponentReflectionData {
        let rawProps = code.groups(for: "\\s*(public\\s+)(var)\\s+([a-zA-Z0-9]+)\\s*:\\s*([a-zA-Z0-9]+)")
        var props: [(String, MetaDataPropertyType)] = []
        
        for prop in rawProps {
            props.append((
                prop[3],
                MetaDataPropertyType(rawValue: prop[4])!
            ))
        }
        
        return ComponentReflectionData(properties: props)
    }
}
