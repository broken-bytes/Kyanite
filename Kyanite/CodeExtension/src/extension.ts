'use strict';

import * as vscode from 'vscode';

import { EntitiesExplorerProvider } from './EntitiesExplorer';
import { ComponentExplorerProvider } from './ComponentExplorer';
import { JsonOutlineProvider } from './jsonOutline';
import { TestView } from './testView';
import { ScriptBinding, ScriptBindingsBuilderProvider } from './ScriptBindingsBuilder';

export function activate(context: vscode.ExtensionContext) {
	const rootPath = (vscode.workspace.workspaceFolders && (vscode.workspace.workspaceFolders.length > 0))
		? vscode.workspace.workspaceFolders[0].uri.fsPath : undefined;



	// Commands
	
	context.subscriptions.push(vscode.commands.registerCommand('bindings.bindingSelected', (label: string, type: string) => {
       console.log(label, type);
    }));

	// Tree Views
	const entityExplorerProvider = new EntitiesExplorerProvider(rootPath);
	const componentExplorerProvider = new ComponentExplorerProvider(rootPath);
	const bindingsExplorerProvider = new ScriptBindingsBuilderProvider(rootPath);
	vscode.window.registerTreeDataProvider('entity-hierarchy', entityExplorerProvider);
	vscode.window.registerTreeDataProvider('component-outliner', componentExplorerProvider);
	vscode.window.registerTreeDataProvider('bindings', bindingsExplorerProvider);

	vscode.workspace.onDidOpenTextDocument(document => {
		const editor = vscode.window.createTextEditorDecorationType({});
		const activeEditor = vscode.window.activeTextEditor;
		if (activeEditor) {
			const text = activeEditor.document.getText();
			bindingsExplorerProvider.refresh(text);
		} else {
			console.log("No active text editor");
		}
	});

	// Test View
}