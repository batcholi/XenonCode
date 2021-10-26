// IOs
const $altitudeSensor = 1
const $console = 2
const $screen = 3
const $camera = 4

// include "pid.xc"

// Global variables
var $currentTimeSeconds = 0
storage var $a:number
storage array $stuff:text

init // executed only once, when computer is powered on
	// $a = 2 + (1+1) * +2 - (5.0 - (1+3 + -8*0)) // Should result in 5
// 	if $a == 5
// 		output.2 ("Hello, World!")
// 		@test()
// 	else
// 		output.$console ("There must be a bug somewhere...")
	
// 	// Fill the array
// 	$stuff.fill(10, "")
// 	// Set all items in the array to their index
// 	foreach $stuff ($item, $i)
// 		$item = $i:text
// 	// Print the array
// 	foreach $stuff ($item)
// 		output.$console ($item)
	
input.$altitudeSensor ($altitude:number) // executed when receiving data from the sensor
// 	$outputText = "My altitude is " & round($altitude, 2, 0):format(1.2) & " meters"
// 	output.$console($outputText)
	
function @showTotalTime($hours:number, $minutes:number):text
// 	output.$screen("state", "fillColor #ffffff fontSize 0.06")
// 	output.$screen("state", "left 0.01 top 0.01")
// 	output.$screen("text StartThruster", $hours:format(2.0) & ":" & $minutes:format(2.0))

timer frequency 1 // executed one time per second
// 	$currentTimeSeconds++
// 	var $minutes = $currentTimeSeconds / 60
// 	var $hours = $minutes / 60
// 	$minutes.floor()
// 	$hours.floor()
// 	$minutes %= 60
// 	@showTotalTime ($hours, $minutes)

input.$screen($btn:text)
// 	if $btn == "StartThruster"
// 		output.$console("PSHHHHHHH!!!!")

input.$camera($data:data)
// 	output.$screen($data, 5, "test")
