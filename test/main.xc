array $mem: $number

recursive function @fib($val: number): number
	while $mem.size <= $val
		mem.append(-1)
	
	if $val < 0
		return 0
	elseif $val == 0
		return 1
	elseif $mem.$val != -1
		return $mem.$val
	else
		$mem.$val = self($val - 1) + self($val - 2)
		return $mem.$val

init
	$mem.fill(100, -1)
	print($mem.size)
	print(@fib(20))
