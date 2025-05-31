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
	
	; test 19
	if 0.5 == 0
		var $msg = "ERROR"
		$results.append($msg)
	else
		var $msg = "OK"
		$results.append($msg)
	
	; test 20
	$results.append(upper("hello"))
	$results.append(lower("TEST"))
	
	; test 21
	var $txt = "o"
	$txt.substring(0,0)
	if $txt == ""
		$results.append("OK")
	else
		$results.append("ERROR")
	
	; test 22
	for 3,8 ($i)
		$results.append($i)
	
	; test 23
	for -1,-5 ($i)
		$results.append($i)
		
	; test 24
	$results.append(isnumeric("-12364.12"))
	$results.append(isnumeric("123gg64a"))
	$results.append(isnumeric("(546)"))
	$results.append(isnumeric(""))
	
	; test 25
	$results.append(if(1, "OK", 12))
	$results.append(if(0, "OK", 12))
	$results.append(if(1, 15, "OK"))
	
	; test 26
	for 1,1 ($i)
		$results.append($i)
	
	; test 27
	$results.append(sign(2, -1))
	$results.append(sign(-2))
	$results.append(sign(0))
	$results.append(sign(0, 1))

  	; test 28
	var $rpl = "Hello My Friend!"
	$results.append(replace($rpl, "Friend!", "XenonCode",0))
	$rpl.replace("e", "E", 1)
	$results.append($rpl)

	; test 29
	$results.append("Test 28 - Default double to text conversion.")
	$results.append(2)
	$results.append(2.0)
	$results.append(1)
	$results.append(1.0)
	$results.append(0)
	$results.append(1.1)
	$results.append(12.12)
	$results.append(123.123)
	$results.append(1234.1234)
	$results.append(12345.12345)
	$results.append(123456.123456)
	$results.append(1234567.1234567)
	$results.append(12345678.12345678)
	$results.append(123456789.123456789)
	$results.append(12345678.123456789)
	$results.append(1234567.123456789)
	$results.append(123456.123456789)
	$results.append(12345.123456789)
	$results.append(1234.123456789)
	$results.append(123.123456789)
	$results.append(12.123456789)
	$results.append(1.123456789)
	$results.append(0.123456789)
	$results.append(0.0123456789)
	$results.append(0.00123456789)
	$results.append(0.000123456789)
	$results.append(0.0000123456789)
	$results.append(0.00000123456789)
	$results.append(0.000000123456789)
	
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
