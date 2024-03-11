import Foundation

class FilesystemWatchdog {
    struct File: Hashable, Equatable {
        var path: String
        var date: Date
        var isDirectory: Bool

        static func == (lhs: File, rhs: File) -> Bool {
            return lhs.path == rhs.path
        }

        func hash(into hasher: inout Hasher) {
            hasher.combine(path)
        }
    }
    enum FileChange {
        case added
        case modified
        case removed
    }

    var path: String

    var files: [File] = []
    var thread: Thread? = nil

    var onChange: ((File, FileChange) -> Void)

    init(at path: String, onChange: @escaping ((File, FileChange) -> Void)) {
        self.path = path
        self.onChange = onChange
        self.files = getFileList()
        print(files)

        // Call the onChange callback for all the files in the directory at the start
        for file in files {
            print(file)
            onChange(file, .added)
        }
    }

    func watch() {
        let fileManager = FileManager.default
        let url = URL(fileURLWithPath: path)

        thread = Thread {
            while !Thread.current.isCancelled {
                var currentFiles = self.getFileList()
                // Compare the current files with the previous files
                for file in currentFiles {
                    if !self.files.contains(file) {
                        self.onChange(file, .added)
                    } else {
                        // Check if the file was modified
                        guard 
                            let attributes = try? fileManager.attributesOfFileSystem(forPath: file.path)
                        else {
                            continue
                        }
                        let date = attributes[FileAttributeKey.modificationDate] as? Date ?? Date()
                        
                        if date != file.date {
                            self.onChange(file, .modified)
                        }
                    }
                }

                for file in self.files {
                    if !currentFiles.contains(file) {
                        self.onChange(file, .removed)
                    }
                }

                self.files = currentFiles

                Thread.sleep(forTimeInterval: 1)
            }
        }
    }

    func stop() {
        thread?.cancel()
    }

    func getFileList() -> [File] {
        let fileManager = FileManager.default
        let url = URL(fileURLWithPath: path)
        let fileURLs = fileManager.enumerator(at: url, includingPropertiesForKeys: nil)?.allObjects as! [URL]
        print(fileURLs)

        for fileURL in fileURLs {
            guard 
                let attributes = try? fileManager.attributesOfFileSystem(forPath: fileURL.absoluteString)
            else {
                continue
            }
            let date = attributes[FileAttributeKey.modificationDate] as? Date ?? Date()
            files.append(File(path: fileURL.absoluteString, date: date, isDirectory: fileURL.hasDirectoryPath))
        }
        return files
    }
}