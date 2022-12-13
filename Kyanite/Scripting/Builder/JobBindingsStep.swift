//
//  File.swift
//  
//
//  Created by Kulina, Marcel on 12.09.22.
//

import Foundation

extension String {
    func groups(for regexPattern: String) -> [[String]] {
        do {
            let text = self
            let regex = try NSRegularExpression(pattern: regexPattern)
            let matches = regex.matches(in: text,
                                        range: NSRange(text.startIndex..., in: text))
            return matches.map { match in
                return (0..<match.numberOfRanges).map {
                    let rangeBounds = match.range(at: $0)
                    guard let range = Range(rangeBounds, in: text) else {
                        return ""
                    }
                    return String(text[range])
                }
            }
        } catch let error {
            return []
        }
    }
}


class JobBindingsStep: GeneratorStep {
    func setUp() {
        
    }
    
    func run() {
        do {
            var jobs: [JobClass] = []
            
            let path: String = CommandLine.arguments[1]
            
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
                
                for file in files {
                    if file.pathExtension == "swift" {
                        let data = FileManager.default.contents(atPath: file.path)
                        guard let fileData = data else { return }
                        guard let content = String(data: fileData, encoding: .utf8) else { return }
                        let matches = content.groups(for: "(public class)\\s+([a-zA-Z0-9]+)\\s*\\:\\s*(Job)\\s+(\\{(\\s|\\S)*\\})")
                        if !matches.isEmpty {
                            // We have accessor modifier
                            let jobBody = matches[0][4]
                            
                            let exec = jobBody.groups(for: "public func execute\\(([a-zA-Z0-9\\s\\:\\,]+)*\\)")
                            if !exec.isEmpty {
                                let params = exec[0][1].split(separator: ",").compactMap {
                                    return $0.replacingOccurrences(of: " ", with: "").split(separator: ":")
                                }.compactMap {
                                    return (String($0[0]), String($0[1]))
                                }
                                
                                jobs.append(
                                    JobClass(
                                        name: matches[0][2],
                                        properties: params.compactMap {
                                            return Property(name: $0.0, type: $0.1)
                                        }))
                                
                            }
                        }
                        
                    }
                    
                    var generatedCode = ""
                    generatedCode = generatedCode.appending(
                    """
                    import EntityComponentSystem
                    import Profiling
                    import Foundation
                    import Userland
                    
                    var jobs: [Job] = []
                    
                    @_cdecl("AssemblyJobsInit") func initJobSystem() {
                    
                    """
                    )
                    
                    for job in jobs {
                        generatedCode = generatedCode.appending(
"""

    jobs.append(\(job.name)())

""")
                    }
                    
                    generatedCode = generatedCode.appending(
"""

}

""")
                    
                    generatedCode = generatedCode.appending(
"""

@_cdecl("AssemblyJobsUpdate") func executeJobs() {

""")
                    var index = 0
                    for job in jobs {
                        let jobParams = job.properties.map {
                            return "\($0.type).self".replacingOccurrences(of: "\"", with: "")
                        }
                        
                        let paramStr = jobParams.compactMap {
                            return $0
                        }.joined(separator: ", ")
                        
                        let executeParams = job.properties.map {
                            return "\($0.name): &\($0.name)".replacingOccurrences(of: "\"", with: "").appending(".pointee")
                        }
                        
                        let executeStr = executeParams.compactMap {
                            return $0
                        }.joined(separator: ", ")
                        
                        generatedCode.append(
"""

            Task {
            let entities = ECS.shared.filter(types: [\(paramStr)])
            for entity in entities {
"""
                        )
                        for prop in job.properties {
                            generatedCode.append(
"""

                let \(prop.name) = entity.componentOf(type: \(prop.type).self)!

"""
                            )
                        }
                        generatedCode.append(
"""
                (jobs[\(index)] as! \(job.name)).execute(\(executeStr))
            }
        }
        
"""
                        )

                        index += 1
                    }
                    
                    generatedCode.append(
"""
    
}
""")

                    generatedCode = generatedCode.replacingOccurrences(of: "inout", with: "")
                    
                    FileManager.default.createFile(
                        atPath: path.appending("/Cache/_generated_JobSystem.swift"),
                        contents: generatedCode.data(using: .utf8))
                    
                }
            }
        }
        catch {
            print(error.localizedDescription)
        }
    }
    
    func cleanUp() {
        let path = FileManager.default.currentDirectoryPath
        do {
            try? FileManager.default.removeItem(at: URL(fileURLWithPath: path.appending("/Cache/_generated_JobSystem.swift")))
        }
    }
}
