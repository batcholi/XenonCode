const vscode = require('vscode');
const { spawn } = require('child_process');

function activate(context) {
  let diagnosticCollection = vscode.languages.createDiagnosticCollection('xenoncode_line_linter');

  // WARNING Message to use Tabs instead of Spaces
  const editorConfig = vscode.workspace.getConfiguration('editor', { languageId: 'xenoncode' });
  if (editorConfig.get('insertSpaces')) {
    vscode.window.showWarningMessage(
      "For your XenonCode source files to compile properly, you need to use TABS instead of spaces. You may want to set 'editor.insertSpaces' to 'false' in your settings.",
      'Open Settings'
    ).then(selection => {
      if (selection === 'Open Settings') {
        vscode.commands.executeCommand('workbench.action.openSettings', 'editor.insertSpaces');
      }
    });
  }

  // Linter
  const linterExecutable = vscode.workspace.getConfiguration().get('xenoncode.linter.executable', 'xenoncode');
  vscode.workspace.onDidChangeTextDocument(event => {
    
    const document = event.document;
    const lines = document.getText().split('\n');
    const diagnostics = [];
    diagnosticCollection.clear();

    lines.forEach((line, lineIndex) => {
      let args = ['-parse-line-generic', line];
      let linter = spawn(linterExecutable, args);
      linter.stderr.on('data', (data) => {
        console.error(data.toString());
        if (data.length > 0) {
          let range = new vscode.Range(new vscode.Position(lineIndex, 0), new vscode.Position(lineIndex, line.length));
          let diagnostic = new vscode.Diagnostic(range, data.toString(), vscode.DiagnosticSeverity.Error);
          diagnostics.push(diagnostic);
          diagnosticCollection.set(document.uri, diagnostics);
        }
      });
    });
  });

}
exports.activate = activate;
