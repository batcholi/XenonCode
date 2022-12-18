
CodeMirror.defineSimpleMode("xenoncode", {
	// The start state contains the rules that are initially used
	start: [
	  // The regex matches the token, the token property contains the type
	  {regex: /(?:("|\}))([^\\"\{\}]*)(?:("|$|\{))/, token: ["string-2", "string", "string-2"]},
	  {regex: /function|storage|array|var|const|include|return|if|foreach|while|else|elseif|init|input|timer|tick|output|repeat|frequency|interval|break|loop/, token: "keyword"},
	//   {regex: /true|false/, token: "atom"},
	//   {regex: /[-+]?(\d+\.?\d*)(?:e[-+]?\d+)?/i, token: "number"},
	  {regex: /(\d+\.?\d*)/i, token: "number"},
	  {regex: /(\/\/|;).*$/, token: "comment"},
	  {regex: /[-+\/*=<>!&\^%]+/, token: "operator"},
	  // indent and dedent properties guide autoindentation
	  {regex: /\$[a-z_][\w]*/, token: "variable"},
	  {regex: /@[a-z_][\w]*/, token: "variable-2"},
	],
	// The meta property contains global information about the mode. It
	// can contain properties like lineComment, which are supported by
	// all modes, and also directives like dontIndentStates, which are
	// specific to simple modes.
	meta: {
	  dontIndentStates: ["comment"],
	  lineComment: "//"
	}
  });
  