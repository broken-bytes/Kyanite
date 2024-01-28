import SwiftCompilerPlugin
import SwiftSyntax
import SwiftSyntaxBuilder
import SwiftSyntaxMacros
import Foundation

public struct StringifyMacro: ExpressionMacro {
    public static func expansion(
        of node: some FreestandingMacroExpansionSyntax,
        in context: some MacroExpansionContext
    ) -> ExprSyntax {
        guard let argument = node.argumentList.first?.expression else {
            fatalError("compiler bug: the macro does not have any arguments")
        }

        return "(\(argument), \(literal: argument.description))"
    }
}

public struct ComponentMacro: MemberMacro, ExtensionMacro {
    public static func expansion(
        of node: SwiftSyntax.AttributeSyntax, 
        attachedTo declaration: some SwiftSyntax.DeclGroupSyntax, 
        providingExtensionsOf type: some SwiftSyntax.TypeSyntaxProtocol, 
        conformingTo protocols: [SwiftSyntax.TypeSyntax], 
        in context: some SwiftSyntaxMacros.MacroExpansionContext
    ) throws -> [SwiftSyntax.ExtensionDeclSyntax] {
        return [ExtensionDeclSyntax(
            extendedType: type, 
            memberBlock: MemberBlockSyntax(": Component, Hashable {}"))
        ]
    }

    public static func expansion(
        of node: AttributeSyntax, 
        providingMembersOf declaration: some DeclGroupSyntax, 
        conformingTo protocols: [TypeSyntax], 
        in context: some MacroExpansionContext
    ) throws -> [DeclSyntax] {
        guard let structDecl = declaration.as(StructDeclSyntax.self) else {
            fatalError("Component macro can only be used on structs")
        }

        var metadata = "using flecs.meta"
        metadata += "\n\nStruct \(structDecl.name.text) {\n"

        let members = structDecl.memberBlock.members        

        for member in members {
            if let variable = member.as(MemberBlockItemSyntax.self) {
                guard 
                    let variable = variable.decl.as(VariableDeclSyntax.self),
                    let name = variable.bindings.first?.pattern.description,
                    let type = variable.bindings.first?.typeAnnotation?.type.description
                else {
                    fatalError("Component member invalid")
                }

                // Components may only have the following types:
                // - int8, 
                // - int16
                // - int32
                // - int64
                // - float
                // - double
                // - bool
                // Any other type that uses these only these types is also allowed

                switch type {
                    // Builtin types are always allowed
                case "Int8":
                    metadata += "   \(name) :- i8\n"
                    break
                case "Int16":
                    metadata += "   \(name) :- i16\n"
                    break
                case "Int32":
                    metadata += "   \(name) :- i32\n"
                    break
                case "Int64":
                    metadata += "   \(name) :- i64\n"
                    break
                case "Float":
                    metadata += "   \(name) :- f32\n"
                    break
                case "Double":
                    metadata += "   \(name) :- f64\n"
                    break
                case "Bool":
                    metadata += "   \(name) :- bool\n"
                    break
                default:
                    // We just assume that the type is valid because we can't check it
                    metadata += "   \(name) :- \(type)\n"
                }
            }
        }

        metadata += "}"

        // Generate flecs compatible metadata so we can use the component in the rest api and explorer
        let filename = URL(fileURLWithPath: "./meta/\(structDecl.name.text).ecs")
        try? metadata.write(to: filename, atomically: true, encoding: String.Encoding.utf8)
        return []
    }
}

@main
struct ScriptingPlugin: CompilerPlugin {
    let providingMacros: [Macro.Type] = [
        ComponentMacro.self,
    ]
}
