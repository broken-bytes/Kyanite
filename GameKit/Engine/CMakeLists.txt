set(
    SWIFT_SRC
    Sources/GameKit.swift
    Sources/ECS/BitSet.swift
    Sources/ECS/ECSError.swift
    Sources/ECS/EntityComponentSystem.swift
    Sources/ECS/Component.swift
    Sources/ECS/ComponentIdMapping.swift
    Sources/ECS/ComponentPool.swift
    Sources/ECS/ComponentRegistry.swift
    Sources/ECS/ComponentData.swift
    Sources/ECS/Entity.swift
    Sources/ECS/Pipeline.swift
    Sources/ECS/Registry.swift
    Sources/ECS/Archetype.swift
    Sources/ECS/ArchetypePool.swift
    Sources/ECS/System.swift
    Sources/ECS/SystemIterator.swift
    Sources/ECS/TickSource.swift
    Sources/ECS/World.swift
    Sources/ECS/Utils.swift
)

add_library(GameKit SHARED ${SWIFT_SRC})

target_compile_options(GameKit PRIVATE -enable-experimental-cxx-interop)
target_link_libraries(GameKit PRIVATE Core)
target_link_libraries(GameKit PRIVATE Rendering)

    set_target_properties(
        GameKit
        PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        Swift_MODULE_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
