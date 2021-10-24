
var $p = 1
var $i = 1
var $d = 1
var $min = 1
var $max = 1

var $integral:number
var $pre_error:number

input 0 ($_p:number, $_i:number, $_d:number, $_min:number, $_max:number)
	$p = $_p
	$i = $_i
	$d = $_d
	$min = $_min
	$max = $_max

input 0 ($error:number) // $error is setpoint - pv

	// Proportional term
	var $Pout = $p * $error

	// Integral term
	$integral += $error * delta()
	var $Iout = $i * $integral

	// Derivative term
	var $derivative = ($error - $pre_error) / delta()
	var $Dout = $d * $derivative

	// Calculate total output
	var $output = $Pout + $Iout + $Dout

	// Restrict to max/min
	if $output > $max
		$output = $max
	elseif $output < $min
		$output = $min

	// Save error to previous error
	$pre_error = $error

	output 0 ($output)

