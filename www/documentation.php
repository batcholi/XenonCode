<?php
// DEV
define("DEV", $_SERVER['HTTP_HOST'] === 'dev.xenoncode.com');
error_reporting(E_ALL);
ini_set("display_errors", DEV);

require_once("Parsedown.php");
$Parsedown = new Parsedown();
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<title>XenonCode - Lightweight in-game scripting language</title>
	<meta charset="utf-8">
	<meta name="description" content="XenonCode is a lightweight programming language designed for in-game scripting">
	<meta name="keywords" content="XenonCode">
	<meta property="og:title" content="XenonCode - Lightweight in-game scripting language" />
	<meta property="og:description" content="XenonCode is a lightweight programming language designed for in-game scripting">
	<meta property="og:type" content="website" />
	<meta property="og:url" content="https://XenonCode.com/" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<style>
		body {
			background-color: #222;
			color: #ccc;
			font-family: helvetica,arial;
			width: 80%;
			margin: 0 auto;
			padding: 10px;
		}
		code {
			border: solid 1px #444;
			border-radius: 3px;
			padding: 2px 6px;
			margin: 2px;
			font-size: 14px;
			line-height: 22px;
			display: inline-block;
		}
		hr {
			margin: 40px 0;
		}
		a {
			color: #fff;
		}
		
	</style>
</head>
<body>
	<header>
		
	</header>

	<main>
		<?=$Parsedown->text(file_get_contents("../README.md"))?>
	</main>
	
	<hr>
	
	<footer>
		<a target="_blank" href="https://github.com/batcholi/XenonCode">View project on GitHub</a>
	</footer>
</body>
</html>
