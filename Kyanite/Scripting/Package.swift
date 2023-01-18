// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Kyanite",
    products: [
        .library(
            name: "Kyanite-Editor",
            type: .dynamic,
            targets: ["Editor"]
        ),
        .executable(
            name: "Generator",
            targets: ["Generator-Executable"]
        ),
        .executable(
            name: "MetaBuilder",
            targets: ["MetaBuilder-Executable"]
        ),
        .library(
            name: "Kyanite",
            type: .dynamic,
            targets: ["Core", "Audio", "Engine"]
        )
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .executableTarget(
            name: "Generator-Executable",
            path: "./Generator"
        ),

        .target(
            name: "Audio",
            path: "./Audio"
        ),

        .executableTarget(
            name: "MetaBuilder-Executable",
            path: "./MetaBuilder"
        ),

        .target(
            name: "Core",
            path: "./Core"    
        ),

        .target(
            name: "Editor",
            dependencies: ["Core"],
            path: "./Editor"
        ),

        .target(
            name: "Math",
            path: "./Math"    
        ),

        .target(
            name: "Physics",
            dependencies: ["Math"],
            path: "./Physics"    
        ),

        .target(
            name: "Engine",
            dependencies: ["Core", "Audio", "Math", "Physics"],
            path: "./Engine"
        ),
    ]
)
