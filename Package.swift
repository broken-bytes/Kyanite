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
            targets: ["Core", "Audio", "EntityComponentSystem", "Runtime", "Rendering"]
        )
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .executableTarget(
            name: "Generator-Executable",
            path: "./Kyanite/Scripting/Generator"
        ),

        .target(
            name: "Audio",
            path: "./Kyanite/Scripting/Audio"
        ),

        .executableTarget(
            name: "MetaBuilder-Executable",
            path: "./Kyanite/Scripting/MetaBuilder"
        ),

        .target(
            name: "Core",
            path: "./Kyanite/Scripting/Core"    
        ),

        .target(
            name: "Editor",
            dependencies: ["Core"],
            path: "./Kyanite/Scripting/Editor"
        ),
    
        .target(
            name: "EntityComponentSystem",
            dependencies: ["Audio", "Physics", "Rendering"],
            path: "./Kyanite/Scripting/EntityComponentSystem"    
        ),

        .target(
            name: "Math",
            path: "./Kyanite/Scripting/Math"    
        ),

        .target(
            name: "Physics",
            dependencies: ["Math"],
            path: "./Kyanite/Scripting/Physics"    
        ),

        .target(
            name: "Rendering",
            dependencies: [],
            path: "./Kyanite/Scripting/Rendering"
        ),

        .target(
            name: "Runtime",
            dependencies: ["Core"],
            path: "./Kyanite/Scripting/Runtime"
        ),
    ]
)
