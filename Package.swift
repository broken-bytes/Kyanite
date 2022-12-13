// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Kyanite",
    products: [
        .executable(
            name: "Generator",
            targets: ["Generator-Executable"]
        ),
        .executable(
            name: "Builder",
            targets: ["Builder-Executable"]
        ),
        .library(
            name: "Kyanite",
            type: .dynamic,
            targets: ["Audio", "CBindings", "EntityComponentSystem", "Runtime", "Rendering"]
        )
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        // .package(url: /* package url */, from: "1.0.0"),
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
            name: "Builder-Executable",
            path: "./Kyanite/Scripting/Builder"
        ),

        .target(
            name: "CBindings",
            path: "./Kyanite/Scripting/CBindings"
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
