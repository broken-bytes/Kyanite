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
        .library(
            name: "Kyanite",
            type: .dynamic,
            targets: ["Core", "Audio", "Engine"]
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
