function @pid($error:number, $p:number, $i:number, $d:number, $min:number, $max:number, $integral:number, $pre_error:number)
// $error is setpoint - pv
	
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
	
	return $output

