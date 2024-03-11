// swift-tools-version: 5.9.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription
import CompilerPluginSupport

let package = Package(
    name: "scripting",
    platforms: [.macOS(.v13), .iOS(.v17), .tvOS(.v13), .watchOS(.v6), .macCatalyst(.v13)],
    products: [
        .executable(name: "KyaniteEditorApp", targets: ["KyaniteEditorApp"]),
        .library(
            name: "KyaniteEngine",
            type: .dynamic, 
            targets: ["KyaniteEngine"]
        ),
        .library(
            name: "KyaniteEditor", 
            type: .dynamic,
            targets: ["KyaniteEditor"]
        )
    ],
    dependencies: [
        // Depend on the Swift 5.9 release of SwiftSyntax
        .package(url: "https://github.com/apple/swift-syntax.git", from: "509.1.1")
    ],
    targets: [
        // Targets are the basic building blocks of a package, defining a module or a test suite.
        // Targets can depend on other targets in this package and products from dependencies.
        // Macro implementation that performs the source transformation of a macro.
        .macro(
            name: "Macros",
            dependencies: [
                .product(name: "SwiftSyntaxMacros", package: "swift-syntax"),
                .product(name: "SwiftCompilerPlugin", package: "swift-syntax")
            ],
            path: "scripting/macros"
        ),
        // The shared targets
        .target(
            name: "SceneManagement",
            dependencies: [],
            path: "scripting/shared/scenemanagement"
        ),

        .executableTarget(
            name: "KyaniteEditorApp", 
            dependencies: [
                "KyaniteEditor"
            ], 
            path: "scripting/editorapp"
        ),
 
        .target(
            name: "KyaniteEngine", 
            dependencies: ["Macros", "Native", "SceneManagement"],
            path: "scripting/engine",
            swiftSettings: [
                .unsafeFlags(["-enable-library-evolution"])
            ]
        ),

        .target(
            name: "KyaniteEditor", 
            dependencies: [
                "KyaniteEngine", 
                "Native", 
                "SceneManagement",
            ],
            path: "scripting/editor",
            swiftSettings: [
                .unsafeFlags(["-enable-library-evolution"])
            ]
        ),

        .target(
            name: "Bridge",
            path: "scripting/bridge"
        ),

        .target(
            name: "Native",
            dependencies: ["Bridge"],
            path: "scripting/native"
        )
    ]
)
