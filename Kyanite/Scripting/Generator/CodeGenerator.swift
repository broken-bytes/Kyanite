import Foundation 

struct OutFunc {
    var name: String
    var returnType: String
    var params: String
}

class CodeGenerator {
    func generate(for path: String) {
        
        var outHeader = Data()
        outHeader.append("#pragma once\n".data(using: .utf8)!)
        outHeader.append("#include <stdbool.h>\n".data(using: .utf8)!)

        var outSource = Data()
        outSource.append("#include \"Kyanite-Swift.h\"\n".data(using: .utf8)!)
        var outFuncs: [OutFunc] = []

        FileManager.default.enumerator(atPath: path)?.forEach { currentPath in 
            guard let currentPath = currentPath as? String else { fatalError() }
            if currentPath.contains(".hxx") {
                guard let data = FileManager.default.contents(atPath: currentPath) else { fatalError()}
                
                guard let content = String(data: data, encoding: .utf8) else { fatalError()}
                if content.contains("// #SwiftImport") {
                    var outContent = content
                        .replacingOccurrences(of: "// #SwiftImport", with: "")
                        .replacingOccurrences(of: "#pragma once", with: "")
                        .replacingOccurrences(of: "#include <cstddef>", with: "")
                        .replacingOccurrences(of: "#include <cstdint>", with: "#include <stdint.h>")
                        .replacingOccurrences(of: "#include \"[0-9a-zA-Z.]+\"", with: "", options: [.regularExpression])
                        .replacingOccurrences(of: "size_t", with: "uint64_t")
                        .replacingOccurrences(of: "#define DLL_EXPORT __declspec(dllexport)", with: "")
                        .replacingOccurrences(of: "#define DLL_EXPORT", with: "")
                        .replacingOccurrences(of: "DLL_EXPORT", with: "")
                    
                    outHeader.append(outContent.data(using: .utf8)!)

                    let pattern = #"([a-z0-9A-Z_*]*)\s+([a-z0-9A-Z_]+)\((\s*[a-z0-9A-Z_*\s*,]*)\)\s*(\([a-zA-Z0-9:_\(\)]*\))*[a-zA-Z0-9_:\(\)]*(;)"#
                    do {
                        var outContentSrc = ""
                        if let regex = try? Regex(pattern) {
                            let matches = outContent.matches(of: regex)
                            
                            if matches.count > 0 {
                                matches.forEach {
                                    let name = String(outContent[$0[2].range!])
                                    let returnType = String(outContent[$0[1].range!])
                                    let params =  String(outContent[$0[3].range!])

                                    outFuncs.append(OutFunc(name: name, returnType: returnType, params: params))
                                }
                            }         
                        }
                    }
                }
            }
        }

        for function in outFuncs {
            outSource.append("typedef \(function.returnType)(*T\(function.name))(\(function.params));\n".data(using: .utf8)!)
        }

        for function in outFuncs {
            outSource.append("T\(function.name) _\(function.name);\n".data(using: .utf8)!)
        }

        outSource.append("void InitCBindings() {\n".data(using: .utf8)!);
        outSource.append(
"""
    auto lib = LoadLibrary("./Kyanite-Runtime.dll");\n
""".data(using: .utf8)!)

    outFuncs.forEach { 
        outSource.append(
"""
    _\($0.name) = (T\($0.name))GetProcAddress(lib, \"\($0.name)\");\n
"""
.data(using: .utf8)!)
    }

outSource.append("\n}\n".data(using: .utf8)!);

 outFuncs.forEach { 
        outSource.append(
"""
\($0.returnType) \($0.name)(\($0.params)) {
    _\($0.name)(\($0.params.split(separator: ",").flatMap { 
        return $0.split(separator: " ").last
    }.joined(separator: ",")));
}

"""
.data(using: .utf8)!
)
    }


        
        outHeader.append("\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n void InitCBindings();\n#ifdef __cplusplus\n}\n#endif\n".data(using: .utf8)!)


        FileManager.default.createFile(atPath: path.appending("Kyanite/Scripting/CBindings/include/Kyanite-Swift.h"), contents: outHeader)
        FileManager.default.createFile(atPath: path.appending("Kyanite/Scripting/CBindings/Kyanite-Swift.c"), contents: outSource)

    }
}