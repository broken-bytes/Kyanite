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

public struct ComponentMacro: MemberMacro, ExtensionMacro, MemberAttributeMacro {
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

    public static func expansion(of node: SwiftSyntax.AttributeSyntax, attachedTo declaration: some SwiftSyntax.DeclGroupSyntax, providingAttributesFor member: some SwiftSyntax.DeclSyntaxProtocol, in context: some SwiftSyntaxMacros.MacroExpansionContext) throws -> [SwiftSyntax.AttributeSyntax] {
        // Generate attributes when inside the editor
        #if EDITOR        
        guard let structDecl = declaration.as(StructDeclSyntax.self) else {
            fatalError("Component macro can only be used on structs")
        }

        // Now get all members and add attributes if they are public

        let members = structDecl.memberBlock.members        

        var decls: [DeclSyntax] = []
        for member in members {
            if let variable = member.as(MemberBlockItemSyntax.self) {
                guard 
                    let variable = variable.decl.as(VariableDeclSyntax.self),
                    let name = variable.bindings.first?.pattern.description,
                    let type = variable.bindings.first?.typeAnnotation?.type.description,
                    let access = variable.modifiers?.first?.name.description
                else {
                    fatalError("Component member invalid")
                }

                decls.append(DeclSyntax("@Reference()"))
            }
        }

        return decls
        #endif

        // In engine mode we don't want to generate any attributes
        return []
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
        metadata += "\n\n@brief \(structDecl.name.text) component"
        metadata += "Struct \(structDecl.name.text) {\n"

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
                    metadata += "   \(name) :- {i8}\n"
                    break
                case "Int16":
                    metadata += "   \(name) :- {i16}\n"
                    break
                case "Int32":
                    metadata += "   \(name) :- {i32}\n"
                    break
                case "Int64":
                    metadata += "   \(name) :- {i64}\n"
                    break
                case "Float":
                    metadata += "   \(name) :- {f32}\n"
                    break
                case "Double":
                    metadata += "   \(name) :- {f64}\n"
                    break
                case "Bool":
                    metadata += "   \(name) :- {bool}\n"
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

        return [
            DeclSyntax("public init() {}")
        ]
    }
}

public struct SystemMacro: MemberMacro {
    public static func expansion(
        of node: AttributeSyntax, 
        providingMembersOf declaration: some DeclGroupSyntax, 
        conformingTo protocols: [TypeSyntax], 
        in context: some MacroExpansionContext
    ) throws -> [DeclSyntax] {
        guard let classDecl = declaration.as(ClassDeclSyntax.self) else {
            fatalError("System macro can only be used on classes")
        }

        // Get the name of the system
        let name = classDecl.name.text

        classDecl.memberBlock.members.map {
            print($0.decl.syntaxNodeType)
        }

        // Check the parameters of the run function
        guard let runFunc = classDecl.memberBlock.members.first(
            where: {
                $0.decl.as(FunctionDeclSyntax.self) != nil && 
                $0.decl.as(FunctionDeclSyntax.self)!.name.text == "run"
            }
        )?.decl.as(FunctionDeclSyntax.self)
        else {
            fatalError("System lacks a run function. Please provide a function named `run`")
        }

        let runFunctionParameters = runFunc.signature.parameterClause.parameters

        var runCall = ""
        for (index, parameter) in runFunctionParameters.enumerated() {
            runCall += "\(parameter.firstName.text): \("iterator.get(index: \(index))")"
            if index != runFunctionParameters.count - 1 {
                runCall += ", "
            }
        }

        return [
            DeclSyntax(
"""
var iterator: SystemIterator

init() {
    _SystemRegistry.shared._register(name: "\(raw: name)") { [weak self] iter in 
        guard let self else { fatalError("System \(raw: name) was not initialized")}
        guard let iter else { fatalError("System \(raw: name) was not initialized")}

        let iterator = SystemIterator(handle: iter)
        self.iterator = iterator

        run(\(raw: runCall))
    }
}
"""
            )
        ]
    }
}

@main
struct ScriptingPlugin: CompilerPlugin {
    let providingMacros: [Macro.Type] = [
        ComponentMacro.self,
        SystemMacro.self
    ]
}
