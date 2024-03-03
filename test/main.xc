storage array $results:text
var $someVar = 16
array $someArray:number

function @increment($v:number):number
	return $v + 1

function @RunUnitTests()
	
	// Test 1
	$results.append("Test 1")
	repeat 10 ($i)
		$results.append(text("{0.0}",$i+1))
	repeat 4 ($i)
		$results.append(text("{0}",$i+1))
		$results.append($i+3)
		
	// Test 2
	$results.append("Test 2")
	var $a = $someVar * 2
	var $b = $a - 3
	$a.@increment()
	$b++
	$results.append($a, $b)

	// Test 3
	$results.append("Test 3")
	if $a == 33
		$results.append("OK")
		$a = 2
	else
		$results.append("ERROR")
	$results.append($a)
	
	// Test 4
	$results.append("Test 4")
	$someArray.append(1,2,2+1)
	$results.append($someArray.sum)
	$results.append($someArray.avg)
	$results.append($someArray.max)
	$results.append($someArray.min)
	$someArray.sortd()
	$results.append("---")
	foreach $someArray ($item)
		if $item > 0
			$results.append($item)
		else
			$results.append("ERROR")
	$results.append("---")
	$results.append($someArray.size)
	$someArray.clear()
	$results.append($someArray.size)
	
	// Test 5
	$a *=+1
	$a =-2
	$a +=clamp(-delta,-10,+10)
	if $a == -2
		$results.append("OK")
	else
		$results.append("ERROR")
	
	// Test 6
	if delta == 0
		$results.append("OK")
	else
		$results.append("ERROR")
	
	// Test 7
	if delta
		$results.append("ERROR")
	else
		$results.append("OK")
	
	// Test 8
	$results.append("---")
	var $someText = "Hello World!"
	array $words : text
	$words.from($someText, " ")
	foreach $words ($word)
		$results.append($word)
	$words.from($someText)
	foreach $words ($word)
		$results.append($word)
	$someText = "12 48 1"
	array $numbers : number
	$numbers.from($someText, " ")
	foreach $numbers ($number)
		$results.append($number)
	var $aDigit = 5
	$words.from($aDigit)
	$results.append($words.0)
	$someText = "12"
	$numbers.from($someText)
	foreach $numbers ($number)
		$results.append($number)
	$words.from($numbers)
	foreach $words ($word)
		$results.append($word)
	var $serialized = ""
	$serialized.from($numbers, ", ")
	$results.append($serialized)
	
	// Test 9
	$results.append("---")
	var $utf8 = "Il était une fois"
	$results.append($utf8.last)
	$utf8.substring(2,6)
	$results.append($utf8)
	$results.append($utf8.0)
	$results.append($utf8.1)
	$results.append($utf8.2)
	$utf8.3 = "i"
	$words.from($utf8)
	foreach $words ($word)
		$results.append($word)
	$utf8.substring(1)
	$utf8.2 = "à"
	$results.append($utf8)
	$utf8.4 = "è"
	$utf8.0 = "à"
	$results.append($utf8)
	$utf8.substring(1)
	$results.append($utf8)
	$results.append($utf8.last)
	
	// test 10
	repeat 10 ($i)
		repeat 10 ($j)
			$results.append($i)
			$results.append($j)
			break
	
init
	output.0 ("Hello, World!")
	
	// Run Unit tests
	$results.clear()
	@RunUnitTests()
	
	return
	print("this should not be visible because we have returned early")

; timer interval 1
; 	output.0 ("tick")

; shutdown
; 	output.0 ("Shutdown!")
