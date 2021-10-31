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
		header {}
		main {}
		code {
			border: solid 1px #444;
			border-radius: 3px;
			padding: 2px 6px;
			margin: 2px;
			font-size: 14px;
			line-height: 22px;
			display: inline-block;
		}
		
	</style>
</head>
<body>
	<header>
		<a target="_blank" href="https://github.com/batcholi/XenonCode"><img src="data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/PjxzdmcgaGVpZ2h0PSI3MiIgdmlld0JveD0iMCAwIDcyIDcyIiB3aWR0aD0iNzIiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PGcgZmlsbD0ibm9uZSIgZmlsbC1ydWxlPSJldmVub2RkIj48cGF0aCBkPSJNOCw3MiBMNjQsNzIgQzY4LjQxODI3OCw3MiA3Miw2OC40MTgyNzggNzIsNjQgTDcyLDggQzcyLDMuNTgxNzIyIDY4LjQxODI3OCwtOC4xMTYyNDUwMWUtMTYgNjQsMCBMOCwwIEMzLjU4MTcyMiw4LjExNjI0NTAxZS0xNiAtNS40MTA4MzAwMWUtMTYsMy41ODE3MjIgMCw4IEwwLDY0IEM1LjQxMDgzMDAxZS0xNiw2OC40MTgyNzggMy41ODE3MjIsNzIgOCw3MiBaIiBmaWxsPSIjM0U3NUMzIi8+PHBhdGggZD0iTTM1Ljk5ODUsMTIgQzIyLjc0NiwxMiAxMiwyMi43ODcwOTIxIDEyLDM2LjA5NjY0NCBDMTIsNDYuNzQwNjcxMiAxOC44NzYsNTUuNzcxODMwMSAyOC40MTQ1LDU4Ljk1ODQxMjEgQzI5LjYxNDUsNTkuMTc5Nzg2MiAzMC4wNTI1LDU4LjQzNTg0ODggMzAuMDUyNSw1Ny43OTczMjc2IEMzMC4wNTI1LDU3LjIyNTA2ODEgMzAuMDMxNSw1NS43MTAwODYzIDMwLjAxOTUsNTMuNjk5NjQ4MiBDMjMuMzQzLDU1LjE1NTg5ODEgMjEuOTM0NSw1MC40NjkzOTM4IDIxLjkzNDUsNTAuNDY5MzkzOCBDMjAuODQ0LDQ3LjY4NjQwNTQgMTkuMjcwNSw0Ni45NDU0Nzk5IDE5LjI3MDUsNDYuOTQ1NDc5OSBDMTcuMDkxLDQ1LjQ1MDA3NTQgMTkuNDM1NSw0NS40ODAxOTQzIDE5LjQzNTUsNDUuNDgwMTk0MyBDMjEuODQzLDQ1LjY1MDM2NjIgMjMuMTEwNSw0Ny45NjM0OTk0IDIzLjExMDUsNDcuOTYzNDk5NCBDMjUuMjUyNSw1MS42NDU1Mzc3IDI4LjcyOCw1MC41ODIzMzk4IDMwLjA5Niw0OS45NjQ5MDE4IEMzMC4zMTM1LDQ4LjQwNzc1MzUgMzAuOTM0NSw0Ny4zNDYwNjE1IDMxLjYyLDQ2Ljc0MzY4MzEgQzI2LjI5MDUsNDYuMTM1MjgwOCAyMC42ODgsNDQuMDY5MTIyOCAyMC42ODgsMzQuODM2MTY3MSBDMjAuNjg4LDMyLjIwNTI3OTIgMjEuNjIyNSwzMC4wNTQ3ODgxIDIzLjE1ODUsMjguMzY5NjM0NCBDMjIuOTExLDI3Ljc1OTcyNjIgMjIuMDg3NSwyNS4zMTEwNTc4IDIzLjM5MjUsMjEuOTkzNDU4NSBDMjMuMzkyNSwyMS45OTM0NTg1IDI1LjQwODUsMjEuMzQ1OTAxNyAyOS45OTI1LDI0LjQ2MzIxMDEgQzMxLjkwOCwyMy45Mjg1OTkzIDMzLjk2LDIzLjY2MjA0NjggMzYuMDAxNSwyMy42NTE1MDUyIEMzOC4wNCwyMy42NjIwNDY4IDQwLjA5MzUsMjMuOTI4NTk5MyA0Mi4wMTA1LDI0LjQ2MzIxMDEgQzQ2LjU5MTUsMjEuMzQ1OTAxNyA0OC42MDMsMjEuOTkzNDU4NSA0OC42MDMsMjEuOTkzNDU4NSBDNDkuOTEyNSwyNS4zMTEwNTc4IDQ5LjA4OSwyNy43NTk3MjYyIDQ4Ljg0MTUsMjguMzY5NjM0NCBDNTAuMzgwNSwzMC4wNTQ3ODgxIDUxLjMwOSwzMi4yMDUyNzkyIDUxLjMwOSwzNC44MzYxNjcxIEM1MS4zMDksNDQuMDkxNzExOSA0NS42OTc1LDQ2LjEyOTI1NzEgNDAuMzUxNSw0Ni43MjU2MTE3IEM0MS4yMTI1LDQ3LjQ2OTU0OTEgNDEuOTgwNSw0OC45MzkzNTI1IDQxLjk4MDUsNTEuMTg3NzMwMSBDNDEuOTgwNSw1NC40MDg5NDg5IDQxLjk1MDUsNTcuMDA2NzA1OSA0MS45NTA1LDU3Ljc5NzMyNzYgQzQxLjk1MDUsNTguNDQxODcyNiA0Mi4zODI1LDU5LjE5MTgzMzggNDMuNjAwNSw1OC45NTU0MDAyIEM1My4xMyw1NS43NjI3OTQ0IDYwLDQ2LjczNzY1OTMgNjAsMzYuMDk2NjQ0IEM2MCwyMi43ODcwOTIxIDQ5LjI1NCwxMiAzNS45OTg1LDEyIiBmaWxsPSIjRkZGIi8+PC9nPjwvc3ZnPg==" alt="GitHub"></a>
	</header>

	<main>
		<?=$Parsedown->text(file_get_contents("../README.md"))?>
	</main>

	<footer></footer>
</body>
</html>
