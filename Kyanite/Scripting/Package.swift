// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Kyanite",
    products: [
        .library(
            name: "Kyanite",
            type: .dynamic,
            targets: ["Core", "Audio", "Editor", "Engine"]
        )
    ],
    targets: [
        .target(
            name: "Audio",
            path: "./Audio"
        ),

        .target(
            name: "Core",
            path: "./Core"    
        ),

        .target(
            name: "Editor",
            dependencies: ["Core", "SQLite"],
            path: "./Editor"
        ),

        .target(
            name: "SQLite",
            path: "./SQLite"
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
            dependencies: ["Core", "Audio", "Math", "Physics", "Editor"],
            path: "./Engine"
        ),
    ]
)
