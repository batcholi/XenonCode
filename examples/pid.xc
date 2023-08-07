function @pid($_setpoint:number, $_processvalue:number, $_kp:number, $_ki:number, $_kd:number, $_integral:number, $_prev_error:number) : number
	var $_error = $_setpoint - $_processvalue
	var $_dt = delta
	var $_derivative = ($_error - $_prev_error) / $_dt
	$_integral += $_error * $_dt
	$_prev_error = $_error
	return $_kp * $_error + $_ki * $_integral + $_kd * $_derivative
