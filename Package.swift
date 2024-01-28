// swift-tools-version: 5.9.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription
import CompilerPluginSupport

let package = Package(
    name: "scripting",
    platforms: [.macOS(.v13), .iOS(.v17), .tvOS(.v13), .watchOS(.v6), .macCatalyst(.v13)],
    products: [
        .executable(
            name: "KyaniteEngine",
            targets: ["KyaniteEngine"]
        ),
    ],
    dependencies: [
        // Depend on the Swift 5.9 release of SwiftSyntax
        .package(url: "https://github.com/apple/swift-syntax.git", from: "509.0.0"),
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

        // Library that exposes a macro as part of its API, which is used in client programs.
        // For now this is an executable target, but it will be a library target in the future. 
        .executableTarget(
            name: "KyaniteEngine", 
            dependencies: ["Macros"],
            path: "scripting/engine",
            swiftSettings: [
                .unsafeFlags(["-import-objc-header", "./engine/engine/include/engine/Bridge_Engine.h"]),
                .unsafeFlags(["-I", "./engine/shared/include"]),
                .unsafeFlags(["-l./build/engine/engine/Debug/Engine.lib"]),
            ]
        ),
    ]
)
