storage array $results:text
var $someVar = 16
var $constVar = number_one
array $someArray:number
array $mem: number

const $smallconst = 10^-10
const $pi_const = pi
const $two_pi_expr = pi * 2
const $numbers_sum_const = number_one + number_two
const $text_const = test_str1
const $text_combo_const = test_str1 & " | " & test_str2

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

function @makeTextObj():text
	return ".user{dev}.nested{.val{123}}"

function @scaleVec($vec:vec3, $s:number):vec3
	return $vec * $s

function @doubleMat($m:mat2x2):mat2x2
	return $m * 2

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
	$results.append(replace($rpl, "Friend!", "XenonCode"))
	$rpl.replace("e", "E", 1)
	$results.append($rpl)

	; test 29
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

	; test 30
	$results.append(pi)
	$results.append(2pi)
	$results.append(number_one * 2)
	$results.append(number_two * 4)
	$results.append(number_three * 8)
	$results.append(test_str1)
	$results.append(test_str2)

	; test 31 - Verify that partial or non-numeric storage values are converted to numbers.
	$results.append("test31")
	var $test31_arg1 = 42;
	var $test31_result = $test31_arg1 + $results.last
	$results.append($test31_result)
	$results.append("31test")
	$test31_arg1 = 42;
	$test31_result = $test31_arg1 + $results.last
	$results.append($test31_result)
	
	; test 32
	var $kv = ".a{10}.b{20}.c{30}.(U235)O2{40}.d{50}.(U238)O2{60}.88{70}"
	foreach $kv ($key, $value)
		$results.append($key & " : " & $value & " == " & $kv.$key)
		$kv.$key = 0
	foreach $kv ($key, $value)
		$results.append($key & " : " & $value)
	
	; test 33
	var $nested = ".outer{.inner{5}.other{7}}"
	$results.append($nested.outer.inner)
	$nested.outer.inner += 3
	$results.append($nested.outer.inner)
	$nested.outer.inner++
	$results.append($nested.outer.inner)
	var $outerKey = "outer"
	var $innerKey = "inner"
	var $otherKey = "other"
	$results.append($nested.$outerKey.$otherKey)
	$nested.$outerKey.$otherKey = $nested.$outerKey.$otherKey & "!"
	$results.append($nested.$outerKey.$otherKey)
	$nested.$outerKey.$innerKey += 2
	$results.append($nested.$outerKey.$innerKey)
	$results.append($nested)

	; test 34
	$results.append(2*3^2)
	var $smallvar = 10^-10
	$results.append(text("{0.0000000000000}", 10^-10))
	$results.append(text("{0.0000000000000}", $smallconst))
	$results.append(text("{0.0000000000000}", $smallvar))
	
	; test 35
	$results.append(@makeTextObj().user)
	$results.append(@makeTextObj().nested.val)
	var $pos = position()
	$results.append($pos.xyz)
	$results.append($pos.xyz().y * 2)
	$results.append($pos.xyz.z * 2)

	; test 36
	$results.append("test36")
	$results.append($pi_const)
	$results.append($two_pi_expr)
	$results.append($numbers_sum_const)
	$results.append($text_const)
	$results.append($text_combo_const)
	
	; Test 37
	$results.append($constVar)

	; Test 38 - KV foreach with dots in keys
	var $dotted = ".Item.Water{100}.Item.Iron{200}.recipe.OUTPUT{50}"
	foreach $dotted ($key, $value)
		$results.append($key & " : " & $value & " == " & $dotted.$key)
	var $k = "item.water"
	$dotted.$k = 150
	$results.append($dotted)
	$results.append($dotted.$k)
	$results.append($dotted.iron)
	$dotted.iron = 64
	$results.append($dotted.iron)

	; test 39 - Expression-based trailing index for arrays
	array $exprArr : number
	$exprArr.append(10, 20, 30, 40, 50)
	; Read with literal expression
	$results.append($exprArr.(1 + 1))
	; Read with more complex expression
	$results.append($exprArr.(2 * 2))
	; Read with variable in expression
	var $ei = 2
	$results.append($exprArr.($ei + 1))
	; Write with expression index
	$exprArr.(1 + 2) = 99
	$results.append($exprArr.3)
	; Compound assignment with expression index
	$exprArr.(4 - 2) = 100
	$results.append($exprArr.2)
	; Expression index in a larger expression
	$results.append($exprArr.(0 + 1) + $exprArr.(3 - 1))

	; test 40 - Expression-based trailing index for KV objects
	var $exprObj = ".x{10}.y{20}.z{30}"
	; Read with text expression
	var $prefix = "x"
	$results.append($exprObj.($prefix))
	; Write with text expression
	$exprObj.($prefix) = 99
	$results.append($exprObj.x)
	; Compound assignment with text expression
	$exprObj.("x" & "") += 1
	$results.append($exprObj.x)
	; Expression-based text key using concat
	var $keyPart = "z"
	$results.append($exprObj.($keyPart))

	; test 41 - Expression-based trailing index for text (character access)
	var $exprTxt = "ABCDE"
	$results.append($exprTxt.(1 + 1))
	$exprTxt.(2 + 1) = "X"
	$results.append($exprTxt)

	; test 42 - Matrix declaration and component access (xyzw and 0123)
	var $v : vec3
	$v.x = 1
	$v.y = 2
	$v.z = 3
	$results.append($v.x)
	$results.append($v.1)
	$results.append($v.z)

	; test 43 - Matrix 4x4 row/component access
	var $m44 : mat4x4
	$m44.0.x = 1
	$m44.1.y = 1
	$m44.2.z = 1
	$m44.3.w = 1
	$results.append($m44.0.x)
	$results.append($m44.1.y)
	$results.append($m44.3.w)
	$results.append($m44.0.y)

	; test 44 - Swizzling
	var $v2 : vec3
	$v2.x = 10
	$v2.y = 20
	$v2.z = 30
	var $swxy : vec2
	$swxy = $v2.xy
	$results.append($swxy.x)
	$results.append($swxy.y)
	var $swzx : vec2
	$swzx = $v2.zx
	$results.append($swzx.x)
	$results.append($swzx.y)

	; test 45 - Vector add/sub
	var $v3 : vec3
	$v3 = $v + $v2
	$results.append($v3.x)
	$results.append($v3.y)
	$results.append($v3.z)
	var $v3b : vec3
	$v3b = $v3 - $v2
	$results.append($v3b.x)
	$results.append($v3b.y)
	$results.append($v3b.z)

	; test 46 - Scalar multiply and divide
	var $v4 : vec3
	$v4 = $v * 2
	$results.append($v4.x)
	$results.append($v4.y)
	$results.append($v4.z)
	var $v5 : vec3
	$v5 = $v4 / 2
	$results.append($v5.x)
	$results.append($v5.y)
	$results.append($v5.z)

	; test 47 - Dot product and length (normal function calls)
	var $dotResult = dot($v, $v2)
	$results.append($dotResult)
	var $lenResult = length($v)
	$results.append(round($lenResult * 1000) / 1000)

	; test 48 - Normalize (trailing, in-place)
	var $nv : vec3
	$nv.x = 3
	$nv.y = 0
	$nv.z = 4
	$nv.normalize()
	$results.append(round($nv.x * 1000) / 1000)
	$results.append(round($nv.z * 1000) / 1000)

	; test 49 - Cross product (normal function)
	var $cx : vec3
	$cx.x = 1
	$cx.y = 0
	$cx.z = 0
	var $cy : vec3
	$cy.x = 0
	$cy.y = 1
	$cy.z = 0
	var $cz : vec3
	$cz = cross($cx, $cy)
	$results.append($cz.x)
	$results.append($cz.y)
	$results.append($cz.z)
	; also test trailing form
	$cx.x = 1
	$cx.y = 0
	$cx.z = 0
	$cx.cross($cy)
	$results.append($cx.x)
	$results.append($cx.y)
	$results.append($cx.z)

	; test 50 - Transpose (trailing, in-place)
	var $m2 : mat2x2
	$m2.0.x = 1
	$m2.0.y = 2
	$m2.1.x = 3
	$m2.1.y = 4
	$m2.transpose()
	$results.append($m2.0.x)
	$results.append($m2.0.y)
	$results.append($m2.1.x)
	$results.append($m2.1.y)

	; test 51 - Determinant (normal function call)
	$m2.transpose()
	var $det = determinant($m2)
	$results.append($det)

	; test 52 - Inverse (trailing, in-place)
	$m2.inverse()
	$results.append(round($m2.0.x * 100) / 100)
	$results.append(round($m2.0.y * 100) / 100)
	$results.append(round($m2.1.x * 100) / 100)
	$results.append(round($m2.1.y * 100) / 100)

	; test 53 - Matrix * vector (matmul)
	var $mat33 : mat3x3
	$mat33.0.x = 1
	$mat33.0.y = 0
	$mat33.0.z = 0
	$mat33.1.x = 0
	$mat33.1.y = 2
	$mat33.1.z = 0
	$mat33.2.x = 0
	$mat33.2.y = 0
	$mat33.2.z = 3
	var $vin : vec3
	$vin.x = 10
	$vin.y = 20
	$vin.z = 30
	var $vout : vec3
	$vout = $mat33 * $vin
	$results.append($vout.x)
	$results.append($vout.y)
	$results.append($vout.z)

	; test 54 - Extract position from 4x4 transform via swizzle
	var $transform : mat4x4
	$transform.3.x = 100
	$transform.3.y = 200
	$transform.3.z = 300
	var $tpos : vec3
	$tpos = $transform.3.xyz
	$results.append($tpos.x)
	$results.append($tpos.y)
	$results.append($tpos.z)

	; test 55 - Function with matrix argument and return
	var $scaled : vec3
	$scaled = @scaleVec($v, 5)
	$results.append($scaled.x)
	$results.append($scaled.y)
	$results.append($scaled.z)

	; test 56 - Matrix assignment and compound assignment
	var $va : vec3
	$va.x = 1
	$va.y = 2
	$va.z = 3
	var $vb : vec3
	$vb = $va
	$results.append($vb.x)
	$results.append($vb.y)
	$va += $vb
	$results.append($va.x)
	$results.append($va.y)
	$va -= $vb
	$results.append($va.x)
	$va *= 10
	$results.append($va.x)

	; test 57 - Function returning a mat2x2
	var $fm : mat2x2
	$fm.0.x = 1
	$fm.0.y = 2
	$fm.1.x = 3
	$fm.1.y = 4
	var $fm2 : mat2x2
	$fm2 = @doubleMat($fm)
	$results.append($fm2.0.x)
	$results.append($fm2.1.y)

	; test 58 - Non-square matrix declaration and access
	var $nsm : mat2x3
	$nsm.0.x = 1
	$nsm.0.y = 2
	$nsm.0.z = 3
	$nsm.1.x = 4
	$nsm.1.y = 5
	$nsm.1.z = 6
	$results.append($nsm.0.z)
	$results.append($nsm.1.x)

	; test 59 - Non-square matmul: mat2x3 * vec3 = vec2
	var $nsv : vec3
	$nsv.x = 10
	$nsv.y = 20
	$nsv.z = 30
	var $nsout : vec2
	$nsout = $nsm * $nsv
	$results.append($nsout.x)
	$results.append($nsout.y)

	; test 60 - mat2x3 * mat3x2 = mat2x2
	var $nsm2 : mat3x2
	$nsm2.0.x = 1
	$nsm2.0.y = 4
	$nsm2.1.x = 2
	$nsm2.1.y = 5
	$nsm2.2.x = 3
	$nsm2.2.y = 6
	var $nsr : mat2x2
	$nsr = $nsm * $nsm2
	$results.append($nsr.0.x)
	$results.append($nsr.0.y)
	$results.append($nsr.1.x)
	$results.append($nsr.1.y)

	; test 61 - Identity trailing function
	var $im : mat3x3
	$im.0.x = 99
	$im.1.z = 42
	$im.identity()
	$results.append($im.0.x)
	$results.append($im.0.y)
	$results.append($im.1.y)
	$results.append($im.2.z)

	; test 62 - matN shorthand for matNxN
	var $sm : mat3
	$sm.1.y = 7
	$results.append($sm.1.y)

	; test 63 - xyzw as row accessor on 2D matrix
	var $rm : mat4x4
	$rm.w.w = 42
	$rm.x.y = 13
	$results.append($rm.w.w)
	$results.append($rm.x.y)

	; test 64 - /= compound assignment
	var $dv : vec3
	$dv.x = 10
	$dv.y = 20
	$dv.z = 30
	$dv /= 5
	$results.append($dv.x)
	$results.append($dv.y)
	$results.append($dv.z)

	; test 65 - normalize as standard function (returns new, doesn't modify original)
	var $nv2 : vec3
	$nv2.x = 3
	$nv2.y = 0
	$nv2.z = 4
	var $nv2r : vec3
	$nv2r = normalize($nv2)
	$results.append(round($nv2r.x * 1000) / 1000)
	$results.append(round($nv2r.z * 1000) / 1000)
	$results.append($nv2.x)

	; test 66 - transpose as standard function
	var $tm : mat2x2
	$tm.0.x = 1
	$tm.0.y = 2
	$tm.1.x = 3
	$tm.1.y = 4
	var $tmr : mat2x2
	$tmr = transpose($tm)
	$results.append($tmr.0.y)
	$results.append($tmr.1.x)
	$results.append($tm.0.y)

	; test 67 - inverse as standard function
	var $ivm : mat2x2
	$ivm.0.x = 1
	$ivm.0.y = 2
	$ivm.1.x = 3
	$ivm.1.y = 4
	var $ivmr : mat2x2
	$ivmr = inverse($ivm)
	$results.append(round($ivmr.0.x * 100) / 100)
	$results.append(round($ivmr.1.y * 100) / 100)
	$results.append($ivm.0.x)

	; test 68 - distance
	var $da : vec3
	$da.x = 1
	$da.y = 0
	$da.z = 0
	var $db : vec3
	$db.x = 0
	$db.y = 1
	$db.z = 0
	$results.append(round(distance($da, $db) * 1000) / 1000)

	; test 69 - angle
	$results.append(round(angle($da, $db) * 1000) / 1000)
	var $dc : vec3
	$dc.x = 2
	$dc.y = 0
	$dc.z = 0
	$results.append(angle($da, $dc))

	; test 70 - lerp (normal function)
	var $la : vec3
	$la.x = 0
	$la.y = 0
	$la.z = 0
	var $lb : vec3
	$lb.x = 10
	$lb.y = 20
	$lb.z = 30
	var $lr : vec3
	$lr = lerp($la, $lb, 0.5)
	$results.append($lr.x)
	$results.append($lr.y)
	$results.append($lr.z)

	; test 71 - lerp (trailing, in-place)
	var $lt : vec3
	$lt.x = 0
	$lt.y = 10
	$lt.z = 20
	var $ltb : vec3
	$ltb.x = 10
	$ltb.y = 20
	$ltb.z = 30
	$lt.lerp($ltb, 0.25)
	$results.append($lt.x)
	$results.append($lt.y)
	$results.append($lt.z)

	; test 72 - vec4 with .w component
	var $v4d : vec4
	$v4d.x = 1
	$v4d.y = 2
	$v4d.z = 3
	$v4d.w = 4
	$results.append($v4d.w)
	$results.append($v4d.3)

	; test 73 - 3 and 4-char swizzles
	var $sw3 : vec3
	$sw3 = $v2.xyz
	$results.append($sw3.x)
	$results.append($sw3.y)
	$results.append($sw3.z)
	$sw3 = $v2.zyx
	$results.append($sw3.x)
	$results.append($sw3.y)
	$results.append($sw3.z)

	; test 74 - scalar on left side of multiply
	var $slm : vec3
	$slm.x = 1
	$slm.y = 2
	$slm.z = 3
	var $slr : vec3
	$slr = 3 * $slm
	$results.append($slr.x)
	$results.append($slr.y)
	$results.append($slr.z)

	; test 75 - lerp at boundaries t=0 and t=1
	var $l0a : vec2
	$l0a.x = 0
	$l0a.y = 0
	var $l0b : vec2
	$l0b.x = 10
	$l0b.y = 20
	var $l0r : vec2
	$l0r = lerp($l0a, $l0b, 0)
	$results.append($l0r.x)
	$results.append($l0r.y)
	$l0r = lerp($l0a, $l0b, 1)
	$results.append($l0r.x)
	$results.append($l0r.y)

	; test 76 - scalar matrix/vector elements inside arithmetic expressions
	; (element [r][0] aliases the matrix's base slot; must be treated as scalar, not as a matrix)
	var $em : mat3x3
	$em.0.x = 5
	$em.0.y = 3
	$em.1.x = 10
	$em.1.y = 4
	var $ed : number
	$ed = $em.0.x - $em.0.y
	$results.append($ed)
	$ed = $em.0.x + $em.0.y
	$results.append($ed)
	$ed = $em.1.x - $em.1.y
	$results.append($ed)
	$ed = $em.0.x * $em.0.y
	$results.append($ed)
	$ed = $em.0.x - $em.1.y
	$results.append($ed)
	var $ev : vec3
	$ev.x = 7
	$ev.y = 2
	$ed = $ev.x - $ev.y
	$results.append($ed)

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
