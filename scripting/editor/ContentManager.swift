@_implementationOnly import Native
import Foundation

#if os(Windows)
import WinSDK
#endif

class ContentManager {
    enum ContentType: RawRepresentable {
        enum ContentTypeError: Error {
            case unknownFileType(String)
        }

        case image
        case model
        case audio
        case shader
        case script
        case unknown

        init(rawValue: String) {
            switch rawValue {
            case "png", "jpg", "jpeg", "bmp", "tga", "gif", "psd", "hdr", "pic", "ppm", "pgm", "pbm", "pnm", "exr", "dds", "ktx", "astc", "pvr", "basis":
                self = .image
            case "fbx":
                self = .model
            case "wav", "mp3", "ogg", "flac", "aiff", "aif", "aifc", "caf", "m4a", "mp4", "3gp", "3g2", "amr", "aac", "wma", "alac", "ape", "dsd", "pcm", "dts", "ac3", "eac3", "mlp", "dolby", "dolbyatmos", "dts-hd", "dts-x", "dolbytruehd", "dolbydigitalplus", "dolbydigital":
                self = .audio
            case "hlsl":
                self = .shader
            case "swift":
                self = .script
            default:
                self = .unknown
            }
        }

        var rawValue: String {
            switch self {
                case .image:
                    return "image"
                case .model:
                    return "model"
                case .audio:
                    return "audio"
                case .shader:
                    return "shader"
                case .script:
                    return "script"
                case .unknown:
                    return "unknown"
            }
        }
    }

    private var contentWatchdog: FilesystemWatchdog? = nil
    private var assetDatabase: AssetDatabase

    public private(set) var path: String = ""
    var onChange: [((FilesystemWatchdog.File, FilesystemWatchdog.FileChange) -> Void)] = []
    var files: [FilesystemWatchdog.File] {
        return contentWatchdog?.files ?? []
    }

    init(at path: String, with database: AssetDatabase) {
        let contentPath = path.appending("/content")
        self.assetDatabase = database
        self.path = contentPath
        
        contentWatchdog = FilesystemWatchdog(at: contentPath) { (file, change) in
            switch change {
            case .added:
                self.handleFileAdded(file: file)
            case .modified:
                self.handleFileModified(file: file)
            case .removed:
                self.handleFileRemoved(file: file)
            }

            for callback in self.onChange {
                callback(file, change)
            }
        }
    }

    func watch() {
        contentWatchdog?.watch()
    }

    private func handleFileAdded(file: FilesystemWatchdog.File) {
        // Check if the file is already in the database
        let uuid = assetDatabase.getUUID(path: file.path) 
        OutputDebugStringA("path: \(file.path)\n")
        OutputDebugStringA("uuid: \(uuid)\n")

        // The file is already in the database, check if it was modified
        if uuid != "" {
            let lastFileModified = self.getFileModificationTime(at: file.path)
            let lastModified = assetDatabase.getLastModified(uuid: uuid)


            if lastFileModified > lastModified {
                assetDatabase.updateAsset(uuid: uuid, time: lastFileModified)
            }

            return
        }

        // Open the file, generate metadata and add it to the database
        // Decide what type of file it is
        switch ContentType(rawValue: (URL(fileURLWithPath: file.path).pathExtension)) {
        case .image:
            handleImage(file: file)
        case .model:
            handleModel(file: file)
        case .audio:
            handleAudio(file: file)
        case .shader:
            handleShader(file: file)
        default:
            break
        }
    }

    private func handleFileModified(file: FilesystemWatchdog.File) {
    }

    private func handleFileRemoved(file: FilesystemWatchdog.File) {
    }

    private func handleModel(file: FilesystemWatchdog.File) {
        let meshList = NativeAssetPipeline.shared.loadModel(at: file.path)
        // Add the mesh list to the database
        assetDatabase.addAsset(
            uuid: UUID().uuidString, 
            name: file.path, 
            path: file.path, 
            type: ContentType.model.rawValue, 
            time: Int64(Date().timeIntervalSince1970)
        ) 
    }

    private func handleSound(file: FilesystemWatchdog.File) {
    }

    private func handleShader(file: FilesystemWatchdog.File) {
    }

    private func handleScript(file: FilesystemWatchdog.File) {
    }

    private func handleUnknown(file: FilesystemWatchdog.File) {
    }

    private func handleImage(file: FilesystemWatchdog.File) {
    }

    private func handleAudio(file: FilesystemWatchdog.File) {
    }

    private func getFileModificationTime(at path: String) -> Int64 {
        #if os(Windows)
        var fileTime = FILETIME()
        var attributes = WIN32_FILE_ATTRIBUTE_DATA()

        if GetFileAttributesExA(path, GetFileExInfoStandard, &attributes) {
            fileTime = attributes.ftLastWriteTime;

            var uli: ULARGE_INTEGER = ULARGE_INTEGER()
            uli.LowPart = fileTime.dwLowDateTime;
            uli.HighPart = fileTime.dwHighDateTime;

            // Convert FILETIME to time_t (UNIX timestamp)
            let unixTimestamp = Int64((uli.QuadPart - 116444736000000000) / 10000000)
            
            return unixTimestamp
        } else {
            return 0
        }
        #else

        let attributes = try? FileManager.default.attributesOfFileSystem(forPath: path)
        let date = attributes?[FileAttributeKey.modificationDate] as? Date ?? Date()
        return Int64(date.timeIntervalSince1970.rounded())
        #endif
    }
}