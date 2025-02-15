const vscode = require('vscode');
const { spawn } = require('child_process');

function activate(context) {
  console.log("XenonCode extension loading...");

  let config = vscode.workspace.getConfiguration('xenoncode');
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

  // Debounce function
  function debounce(func, wait, immediate) {
    var timeout;
    return function() {
      var context = this, args = arguments;
      var later = function() {
        timeout = null;
        if (!immediate) func.apply(context, args);
      };
      var callNow = immediate && !timeout;
      clearTimeout(timeout);
      timeout = setTimeout(later, wait);
      if (callNow) func.apply(context, args);
    };
  }

  // Linter / Formatter
  const linter_executable = config.get('linter_executable');
  const linter = debounce((document) => {
    const lines = document.getText().split('\n');
    const diagnostics = [];
    diagnosticCollection.clear();
    lines.forEach((line, lineIndex) => {
      let range = new vscode.Range(new vscode.Position(lineIndex, 0), new vscode.Position(lineIndex, line.length));

      // Replace spaces with tabs
      const editor = vscode.window.activeTextEditor;
      editor.edit(editBuilder => {
        if (line.match(/(^\t*)    /)) {
          let text = line.replace(/(^\t*)    /g, function(fullMatch, group1){
            return group1 + '\t';
          });
          editBuilder.replace(range, text);
          line = text;
        }
      });
      editor.edit(editBuilder => {
        if (line.match(/(^\t*)  /)) {
          let text = line.replace(/(^\t*)  /g, function(fullMatch, group1){
            return group1 + '\t';
          });
          editBuilder.replace(range, text);
          line = text;
        }
      });

      // Run linter
      let args = ['-parse-line-generic', line];
      let linter = spawn(linter_executable, args);
      linter.stderr.on('data', (data) => {
        console.error(data.toString());
        if (data.length > 0) {
          let diagnostic = new vscode.Diagnostic(range, data.toString(), vscode.DiagnosticSeverity.Error);
          diagnostics.push(diagnostic);
          diagnosticCollection.set(document.uri, diagnostics);
        }
      });
    });
  }, 250); // Wait for 250ms before running linter again

  vscode.workspace.onDidChangeTextDocument(event => {
    const document = event.document;
    if (document.languageId !== 'xenoncode') return;
    linter(document);
  });

  console.log("XenonCode extension activated");
}
exports.activate = activate;
