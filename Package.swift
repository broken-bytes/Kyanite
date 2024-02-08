// swift-tools-version: 5.9.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription
import CompilerPluginSupport

let package = Package(
    name: "scripting",
    platforms: [.macOS(.v13), .iOS(.v17), .tvOS(.v13), .watchOS(.v6), .macCatalyst(.v13)],
    products: [
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
        .package(url: "https://github.com/apple/swift-syntax.git", from: "509.1.1"),
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
 
        .target(
            name: "KyaniteEngine", 
            dependencies: ["Macros", "Native"],
            path: "scripting/engine"
        ),

        .target(
            name: "KyaniteEditor", 
            dependencies: ["KyaniteEngine"],
            path: "scripting/editor"
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
