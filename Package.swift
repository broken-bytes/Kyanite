// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Kyanite",
    products: [
        .library(
            name: "Editor",
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
            targets: ["Audio", "CBindings", "EntityComponentSystem", "Runtime", "Rendering"]
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
            name: "CBindings",
            path: "./Kyanite/Scripting/CBindings"
        ),

        .target(
            name: "Editor",
            path: "./Kyanite/Scripting/Editor"
        ),
    
        .target(
            name: "EntityComponentSystem",
            dependencies: ["Audio", "CBindings", "Physics", "Rendering"],
            path: "./Kyanite/Scripting/EntityComponentSystem"    
        ),

        .target(
            name: "Math",
            path: "./Kyanite/Scripting/Math"    
        ),

        .target(
            name: "Physics",
            dependencies: ["CBindings", "Math"],
            path: "./Kyanite/Scripting/Physics"    
        ),

        .target(
            name: "Rendering",
            dependencies: ["CBindings"],
            path: "./Kyanite/Scripting/Rendering"
        ),

        .target(
            name: "Runtime",
            dependencies: ["CBindings"],
            path: "./Kyanite/Scripting/Runtime"
        ),
    ]
)
