// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "HomeSwiftHome",
    products: [
        .library(name: "HomeSwiftHome", targets: ["App"]),
    ],
    dependencies: [
        // 💧 A server-side Swift web framework.
        .package(url: "https://github.com/vapor/vapor.git", from: "3.0.0"),
        
        // HTML renderer
        .package(url: "https://github.com/vapor/leaf.git", .upToNextMinor(from: "3.0.0")),
        
        // 🔵 Swift ORM (queries, models, relations, etc) built on SQLite 3.
        .package(url: "https://github.com/vapor/fluent-postgresql.git", .upToNextMinor(from: "1.0.0")),
        
        // Authentication package
        .package(url: "https://github.com/vapor/auth.git", from:"2.0.0-rc.3.1")
        
        // Homekit Apple Protocol
            .package(url: )
    ],
    targets: [
        .target(name: "App", dependencies: ["FluentPostgreSQL", "Vapor", "Leaf", "Authentication", "HAP"]),
        .target(name: "Run", dependencies: ["App"]),
        .testTarget(name: "AppTests", dependencies: ["App"])
    ]
)

