storage array $results:text
var $someVar = 16
array $someArray:number
array $mem: number

recursive function @fib($n: number): number
	while $mem.size <= $n
		$mem.append(-1)
	if $n < 1
		return 0
	elseif $n == 1
		return 1
	elseif $mem.$n != -1
		return $mem.$n
	else
		$mem.$n = recurse($n - 1) + recurse($n - 2)
		return $mem.$n

function @increment($v:number):number
	return $v + 1

function @RunUnitTests()
	
	; Test 1
	$results.append("Test 1")
	repeat 10 ($i)
		$results.append(text("{0.0}",$i+1))
	repeat 4 ($i)
		$results.append(text("{0}",$i+1))
		$results.append($i+3)
		
	; Test 2
	$results.append("Test 2")
	var $a = $someVar * 2
	var $b = $a - 3
	$a.@increment()
	$b++
	$results.append($a, $b)

	; Test 3
	$results.append("Test 3")
	if $a == 33
		$results.append("OK")
		$a = 2
	else
		$results.append("ERROR")
	$results.append($a)
	
	; Test 4
	$results.append("Test 4")
	$someArray.append(1,2,2+1)
	$results.append($someArray.sum)
	$results.append($someArray.avg)
	$results.append($someArray.max)
	$results.append($someArray.min)
	$someArray.sortd()
	$results.append("---")
	foreach $someArray ($index, $item)
		if $item > 0
			$results.append($item)
		else
			$results.append("ERROR")
	$results.append("---")
	$results.append($someArray.size)
	$someArray.clear()
	$results.append($someArray.size)
	
	; Test 5
	$a *=+1
	$a =-2
	$a +=clamp(-delta,-10,+10)
	if $a == -2
		$results.append("OK")
	else
		$results.append("ERROR")
	
	; Test 6
	if delta == 0
		$results.append("OK")
	else
		$results.append("ERROR")
	
	; Test 7
	if delta
		$results.append("ERROR")
	else
		$results.append("OK")
	
	; Test 8
	$results.append("---")
	var $someText = "Hello World!"
	array $words : text
	$words.from($someText, " ")
	foreach $words ($index, $word)
		$results.append($word)
	$words.from($someText)
	foreach $words ($index, $word)
		$results.append($word)
	$someText = "12 48 1"
	array $numbers : number
	$numbers.from($someText, " ")
	foreach $numbers ($index, $number)
		$results.append($number)
	var $aDigit = 5
	$words.from($aDigit)
	$results.append($words.0)
	$someText = "12"
	$numbers.from($someText)
	foreach $numbers ($index, $number)
		$results.append($number)
	$words.from($numbers)
	foreach $words ($index, $word)
		$results.append($word)
	var $serialized = ""
	$serialized.from($numbers, ", ")
	$results.append($serialized)
	
	; Test 9
	$results.append("---")
	var $utf8 = "Il était une fois"
	$results.append(last($utf8))
	$utf8.substring(2,6)
	$results.append($utf8)
	$results.append($utf8.0)
	$results.append($utf8.1)
	$results.append($utf8.2)
	$utf8.3 = "i"
	$words.from($utf8)
	foreach $words ($index, $word)
		$results.append($word)
	$utf8.substring(1)
	$utf8.2 = "à"
	$results.append($utf8)
	$utf8.4 = "è"
	$utf8.0 = "à"
	$results.append($utf8)
	$utf8.substring(1)
	$results.append($utf8)
	$results.append(last($utf8))
	
	; test 10
	repeat 10 ($i)
		repeat 10 ($j)
			$results.append($i)
			$results.append($j)
			break
	
	; test 11
	$results.append("Hello\n\""World\!""\\")
	
	; test 12
	var $text1 = ".a{0.12}.b{a}"
	var $text2 = ".cc{}.dd{1.53}"
	$results.append(text("{}\n{}", $text1, $text2))
	
	; test 13
	var $ore = "Fe"
	var $composition = ".Al{0.1}.Fe{0.15}.Cu{0.01}"
	$composition.substring(find($composition, "."&$ore&"{") + size($ore) + 2)
	if contains($composition, "}")
		$composition.substring(0, find($composition, "}"))
	$results.append($composition)
	
	; text 14
	var $obj = ".a{1.2}.b{2}.c{}.d{44}"
	$obj.a += 2
	$obj.u = $obj.d
	$results.append($obj.a)
	var $aa = "a"
	var $dd = "d"
	$obj.$aa += $obj.$dd
	$results.append($obj.a)
	$results.append($obj.u)
	
	; test 15
	foreach $obj ($key, $value)
		$results.append($key & " : " & $value)
	
	; test 16
	$obj.$aa++
	$results.append($obj.$aa)
	$obj.$dd!!
	$results.append($obj.$dd)
	$obj.$dd!!
	$results.append($obj.$dd)
	$obj.a++
	$results.append($obj.a)
	$obj.a!!
	$results.append($obj.a)
	$obj.a!!
	$results.append($obj.a)
	
	; test 17
	$results.append(@fib(10))
	
	; test 18
	$obj = ""
	$obj.a1 = 0
	$obj.c4 = 3
	$obj.d5 = ".a2{44}.b33{55}"
	$obj.a2 = 1
	$obj.b33 = 2
	$obj.dd66 = ".a2{66}"
	$results.append($obj.a2)
	$obj.b33++
	$results.append($obj.b33)
	$results.append($obj)
	$results.append($obj.d5)
	$obj.d5 = 1
	$obj.dd66 = 0
	foreach $obj ($key, $value)
		$obj.$key *= 10
	$results.append($obj)
	foreach $obj ($key, $value)
		$obj.$key = ".a{100}.b{200}"
	$results.append($obj)
	foreach $obj ($key, $values)
		foreach $values ($subkey, $value)
			var $subObj = $obj.$key
			$subObj.$subkey /= 100
			$obj.$key = $subObj
	$results.append($obj)
	
init
	output.0 ("Hello, World!")
	
	; Run Unit tests
	$results.clear()
	if $someVar != 16
		return
	@RunUnitTests()
	
	return
	print("this should not be visible because we have returned early")

; timer interval 1
; 	output.0 ("tick")

; shutdown
; 	output.0 ("Shutdown!")
