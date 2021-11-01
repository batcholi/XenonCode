storage array $coucou:text
array $nu:number

init
// 	var $a = "12345"
// 	output.0 ($a.last)
	$nu.clear()
	$nu.append(1,2,3,4,5,6)
	$nu.erase(3,5)
	
	foreach $nu($i)
		output.0($i)
	

	
	//erase

	