const regex = /((@EditorExposed)\s(public)\s(weak)\s(var)\s)([a-zA-Z]+[a-zA-Z0-9]*)\s*:\s*([a-zA-Z]+[a-zA-Z0-9]*)!/gi;

import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

export class ScriptBindingsBuilderProvider implements vscode.TreeDataProvider<ScriptBinding> {

	private _onDidChangeTreeData: vscode.EventEmitter<ScriptBinding | undefined | void> = new vscode.EventEmitter<ScriptBinding | undefined | void>();
	readonly onDidChangeTreeData: vscode.Event<ScriptBinding | undefined | void> = this._onDidChangeTreeData.event;
	private _onDidChangeSelection: vscode.EventEmitter<ScriptBinding | undefined | void> = new vscode.EventEmitter<ScriptBinding | undefined | void>();
	readonly onDidChangeSelection: vscode.Event<ScriptBinding | undefined | void> = this._onDidChangeSelection.event;


	constructor(private workspaceRoot: string | undefined) {
	}

	refresh(fileContent: string): void {
		this._onDidChangeTreeData.fire();
	}

	getTreeItem(element: ScriptBinding): vscode.TreeItem {
		return element;
	}

	getChildren(element?: ScriptBinding): Thenable<ScriptBinding[]> {
		return new Promise((resolve, reject) => {
			const str = vscode.window.activeTextEditor?.document.getText();

			if(str == undefined) {
				resolve([]);
			}
			const bindings: Array<ScriptBinding> = [];
			const matches = str!.matchAll(regex);

			for (const match of matches) {
				console.log("letter before:" + match);
				bindings.push(new ScriptBinding(match[6], match[7], vscode.TreeItemCollapsibleState.None, undefined));

			}

			resolve(bindings);
		});
	}

	private pathExists(p: string): boolean {
		try {
			fs.accessSync(p);
		} catch (err) {
			return false;
		}

		return true;
	}
}

export class ScriptBinding extends vscode.TreeItem {

	constructor(
		public readonly label: string,
		public readonly type: string,
		public readonly collapsibleState: vscode.TreeItemCollapsibleState,
		public readonly content?: string
	) {
		super(label, collapsibleState);

		this.tooltip = `${label}: ${type} ${content ? content : 'Not assigned'}`;
		this.description = `${content ? content : 'Not assigned'}`;

		this.command = {
			command: 'bindings.bindingSelected',
			title: `${label}: ${type}`,
			arguments: [label, type]
		};
		
		if (content != undefined) {
			this.iconPath = {
				light: path.join(__filename, '..', '..', 'resources', 'light', 'binding.svg'),
				dark: path.join(__filename, '..', '..', 'resources', 'dark', 'binding.svg')
			};
		} else {
			this.iconPath = {
				light: path.join(__filename, '..', '..', 'resources', 'light', 'binding-empty.svg'),
				dark: path.join(__filename, '..', '..', 'resources', 'dark', 'binding-empty.svg')
			};
		}
	}

	contextValue = 'dependency';
}
