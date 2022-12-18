<?php
$RUN_TIME_LIMIT = 10; // seconds

// DEV
define("DEV", $_SERVER['HTTP_HOST'] === 'dev.xenoncode.com');
error_reporting(E_ALL);
ini_set("display_errors", DEV);

// PHP Configuration
set_time_limit(1);
ob_implicit_flush(true);

// Project Setup
if (empty($_GET['PROJECT'])) {header("Location: /".uniqid());exit;}
$PROJECT = $_GET['PROJECT'];
if (!preg_match("#^\w+$#", $PROJECT) || strlen($PROJECT) > 100) exit;
$PROJECT_DIR = "../projects/$PROJECT";

// Ajax Load
if (!empty($_GET['loadfile'])) {
	if (preg_match("#^\w+\.xc$#", $_GET['loadfile'])) {
		if (is_dir($PROJECT_DIR)) {
			$filepath = "$PROJECT_DIR/".$_GET['loadfile'];
			if (is_file($filepath)) {
				readfile($filepath);
			}
		} else if ($_GET['loadfile'] == 'main.xc') {
			readfile("../examples/helloworld.xc");
		}
	}
	exit;
}

// Ajax Save
if (!empty($_GET['savefile']) && isset($_POST['content'])) {
	if (preg_match("#^\w+\.xc$#", $_GET['savefile'])) {
		if (!is_dir($PROJECT_DIR)) {
			mkdir($PROJECT_DIR, 0777, true);
			chmod($PROJECT_DIR, 0777);
		}
		$filepath = "$PROJECT_DIR/".$_GET['savefile'];
		file_put_contents($filepath, $_POST['content']);
		chmod($filepath, 0666);
	}
	exit;
}

// Ajax Run
if (isset($_GET['run'])) {
	if (is_dir($PROJECT_DIR)) {
		chdir($PROJECT_DIR);
		set_time_limit($RUN_TIME_LIMIT);
		ob_implicit_flush(false);
		// system("timeout ".$RUN_TIME_LIMIT."s ../../build/xenoncode -compile . -hz 20 -run . 2>&1", $resultCode);
		$handle = popen("timeout ".$RUN_TIME_LIMIT."s ../../build/xenoncode -compile . -hz 20 -run . 2>&1", 'r');
		while (!feof($handle)) {
			echo fgets($handle);
			flush();
		}
		pclose($handle);
	}
	exit;
}

// File
$FILE = !empty($_GET['file'])? $_GET['file'] : 'main.xc';
if (!preg_match("#^\w+\.xc$#", $FILE)) exit;

// CodeMirror
$codemirror_version = "5.63.3";
$codemirror_baseurl = "https://cdnjs.cloudflare.com/ajax/libs/codemirror/".$codemirror_version;
$codemirror_theme = "XenonCode";
$codemirror_keymap = "sublime";
$codemirror_keymaps = ['sublime', 'emacs', 'vim'];
$codemirror_themes = [
	'XenonCode',
	'3024-day',
	'3024-night',
	'abbott',
	'abcdef',
	'ambiance',
	'ambiance-mobile',
	'ayu-dark',
	'ayu-mirage',
	'base16-dark',
	'base16-light',
	'bespin',
	'blackboard',
	'cobalt',
	'colorforth',
	'darcula',
	'dracula',
	'duotone-dark',
	'duotone-light',
	'eclipse',
	'elegant',
	'erlang-dark',
	'gruvbox-dark',
	'hopscotch',
	'icecoder',
	'idea',
	'isotope',
	'juejin',
	'lesser-dark',
	'liquibyte',
	'lucario',
	'material',
	'material-darker',
	'material-ocean',
	'material-palenight',
	'mbo',
	'mdn-like',
	'midnight',
	'monokai',
	'moxer',
	'neat',
	'neo',
	'night',
	'nord',
	'oceanic-next',
	'panda-syntax',
	'paraiso-dark',
	'paraiso-light',
	'pastel-on-dark',
	'railscasts',
	'rubyblue',
	'seti',
	'shadowfox',
	'solarized',
	'ssms',
	'the-matrix',
	'tomorrow-night-bright',
	'tomorrow-night-eighties',
	'ttcn',
	'twilight',
	'vibrant-ink',
	'xq-dark',
	'xq-light',
	'yeti',
	'yonce',
	'zenburn',
];

// Theme
if (!empty($_GET['theme'])) {
	$codemirror_theme = $_GET['theme'];
	setcookie("theme", $_GET['theme'], time()+30*3600*24);
} else if (!empty($_COOKIE['theme'])) {
	$codemirror_theme = $_COOKIE['theme'];
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
	<title>XenonCode - The new in-game scripting language</title>
	<meta charset="utf-8">
	<meta name="description" content="XenonCode is a lightweight programming language designed for in-game scripting">
	<meta name="keywords" content="XenonCode">
	<meta property="og:title" content="XenonCode - The new in-game scripting language" />
	<meta property="og:description" content="XenonCode is a lightweight programming language designed for in-game scripting">
	<meta property="og:type" content="website" />
	<meta property="og:url" content="https://XenonCode.com/" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	
	<!-- jQuery -->
	<script src="https://code.jquery.com/jquery-3.3.1.min.js" integrity="sha256-FgpCb/KJQlLNfOu91ta32o/NMZxltwRo8QtmkMRdAu8=" crossorigin="anonymous"></script>
	
	<!-- Font Awesome -->
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css">

	<style>
		:root {
			color-scheme: dark;
		}
		body {
			background-color: #222226;
			color: #cde;
			font-family: helvetica,arial;
			margin: 0 auto;
			padding: 10px;
			overflow: hidden;
		}
		header h1 {
			font-size: 18px;
			font-weight: normal;
		}
		header a {
			color: #cde;
		}
		header span {
			color: #666;
			line-height: 24px;
		}
		main {}
		code {}
		.CodeMirror {
			height: auto;
			overflow: hidden;
		}
		#resizer {
			width: 50%;
			height: 10px;
			user-select: none;
			background-color: #333;
			margin: 4px 25%;
			border-radius: 10px;
		}
		pre#output {
			overflow-y: scroll;
			min-height: 16px;
			width: 120%;
		}
		footer {}
		br {clear:both;}
		
		/* Loading Spinner */
		i.fa-spinner {animation: loadingSpinner 1s linear infinite;}
		@keyframes loadingSpinner {100% {transform:rotate(360deg);}}
		
	</style>
</head>
<body>
	<header>
		<h1><b>XenonCode</b> - The new in-game scripting language</h1>
		<a target="_blank" href="/documentation.php">Read the documentation</a>
		<label style="float: right;">Theme
			<select onchange="location = '?theme='+this.value">
				<?php foreach ($codemirror_themes as $theme) {
					echo "<option value=\"$theme\" ".($theme == $codemirror_theme? 'selected':'').">$theme</option>";
				}?>
			</select>
		</label>
		<br><br>
		<!-- <button onclick="Save()" title="ctrl+s">Save</button> -->
		<button onclick="Save(Run)" title="ctrl+r (this also saves the file)">RUN</button> <span> &lt;--- runs the <b>init</b> block in main.xc</span>
		<br>
		<span>
			Try running the program, you will see the output under the editor
			<br>
			You may set your own url, share your code with friends
			<br>
			CTRL+S to save the file, CTRL+R to run the program
			<br>
			Indent using <b>tabs</b> instead of spaces
		</span>
		<label style="float: right;">File
			<select onchange="
				var filename = (this.value || prompt('Enter a filename'));
				$(this).val('<?=$FILE?>');
				if (filename) Save(function(){
					if (filename.match(/^\w+\.xc$/))
						location = '?file='+filename;
					else if (filename.match(/^\w+$/))
						location = '?file='+filename+'.xc';
					else
						alert('Invalid filename. Try not to use special characters.');
				});
			">
				<?php 
				if (!is_file("$PROJECT_DIR/$FILE")) echo "<option value=\"$FILE\">$FILE</option>";
				if (is_dir($PROJECT_DIR)) foreach (glob($PROJECT_DIR."/*.xc") as $file) {
					$filename = basename($file);
					echo "<option value=\"$filename\" ".($filename == $FILE? 'selected':'').">$filename</option>";
				}
				?>
				<option value="">New file</option>
			</select>
		</label>
		<br>
	</header>

	<main>
		<code></code>
		<div id="resizer"></div>
		<pre id="output"></pre>
	</main>

	<footer></footer>

	<!-- CodeMirror -->
	<link rel="stylesheet" href="<?=$codemirror_baseurl?>/codemirror.min.css" crossorigin="anonymous" referrerpolicy="no-referrer" />
	<?php if ($codemirror_theme == "XenonCode") {?>
		<link rel="stylesheet" href="/xc_theme.css" />
	<?php } else {?>
		<link rel="stylesheet" href="<?=$codemirror_baseurl?>/theme/<?=$codemirror_theme?>.min.css" crossorigin="anonymous" referrerpolicy="no-referrer" />
	<?php }?>
	<script src="<?=$codemirror_baseurl?>/codemirror.min.js" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
	<script src="<?=$codemirror_baseurl?>/keymap/<?=$codemirror_keymap?>.min.js" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
	<script src="<?=$codemirror_baseurl?>/addon/mode/simple.min.js" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
	<script src="<?=$codemirror_baseurl?>/addon/comment/comment.min.js" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
	<script src="/xc_mode.js"></script>
	<script>
		var editor = null;
		var heightRatio = 0.85;
		var editorTop = 50;
		
		// Resize
		function ResizeEditor() {
			editorTop = $('code').offset().top + 25;
			var innerHeight = window.innerHeight - editorTop;
			editor.setSize(null, heightRatio * innerHeight);
			$('pre#output').css('height', ((1.0 - heightRatio) * innerHeight - 15) + 'px');
		}
		if (screen.orientation) screen.orientation.onchange = ResizeEditor;
		else document.addEventListener("orientationchange", ResizeEditor);
		window.addEventListener("resize", ResizeEditor);
		window.addEventListener("load", ResizeEditor);
		var resizing = false;
		$('#resizer').on('mousedown', function(){
			resizing = true;
		});
		$(window).on('mouseup', function(){
			resizing = false;
		});
		$(window).on('mousemove', function(){
			if (resizing) {
				var innerHeight = window.innerHeight - editorTop;
				heightRatio = (event.clientY - editorTop) / innerHeight;
				ResizeEditor();
			}
		});
		
		// Load
		$.ajax({
			url: '?loadfile=<?=$FILE?>',
			success: function(content) {
				editor = CodeMirror(document.body.getElementsByTagName("code")[0], {
					value: content,
					lineNumbers: true,
					indentWithTabs: true,
					smartIndent: false,
					mode: "xenoncode",
					tabSize: 4,
					indentUnit: 4,
					keyMap: "<?=$codemirror_keymap?>",
					autoCloseBrackets: true,
					matchBrackets: true,
					lineWrapping: false,
					showCursorWhenSelecting: true,
					theme: "<?=$codemirror_theme?>",
					extraKeys: {
						'Ctrl-/': function(){editor.execCommand('toggleComment')}
					}
				});
				ResizeEditor();
			}
		});
		
		function Save(callback) {
			$.ajax({
				url: '?savefile=<?=$FILE?>',
				method: 'post',
				data: {
					content: editor.getValue()
				},
				success: function(response) {
					if (callback) {
						callback(response);
					}
				}
			});
		}
		
		function Run() {
			var loadingResponseSpinner = '<i id="loading_response_spinner" class="fas fa-spinner"></i>';
			$("#output").html(loadingResponseSpinner);
			$.ajax({
				url: '?run',
				xhr: function() {
					var xhr = new window.XMLHttpRequest();
					xhr.onprogress = function() {
						$('#output > #loading_response_spinner').remove();
						$("#output").append(xhr.responseText);
						$("#output").append(loadingResponseSpinner);
					};
					return xhr;
				},
				success: function(response) {
					$('#output > #loading_response_spinner').remove();
				},
				error: function() {
					$('#output > #loading_response_spinner').remove();
					$("#output").append('ERROR');
				}
			});
		}
		
		$(window).bind('keydown', function(event) {
			if (event.ctrlKey || event.metaKey) {
				switch (String.fromCharCode(event.which).toLowerCase()) {
					case 's':
						event.preventDefault();
						Save();
					break;
					case 'r':
						event.preventDefault();
						Save(Run);
					break;
				}
			}
		});
		
	</script>
</body>
</html>
