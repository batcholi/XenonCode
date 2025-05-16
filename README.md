# XenonCode - Documentation

**XenonCode** is a lightweight programming language designed as high level scripting within games for virtual computing devices (ie: a programmable computer within a game).

# Capabilities

- Typed Variables
- Constants
- Dynamic Arrays
- Standard arithmetic operations on numeric values
- Easy string concatenation and formatting
- User-defined functions
- Device-defined functions and objects
- Trailing/member functions
- Built-in standard math functions
- Built-in IO operations between virtual devices
- Built-in key-value objects
- Synchronized interval functions (timers)
- `if`/`elseif`/`else` conditionals
- `while` loops
- `foreach` loops for arrays
- `repeat n` loops
- `for` loops with first and last index

# Sample code

```xc
include "my_functions.xc"

; Declare global storage
storage var $myStorageVar : number
storage array $myStorageArray : number

; Declare global variables
var $myVar = 0
var $myVar2 = "Hello World"
var $myVar3 : number
var $myVar4 : text
array $myArray : number
array $myArray2 : text

; Declare a user-defined function that changes the values of $myVar and $myVar2
function @myFunction($arg1 : number, $arg2 : text)
	$myVar = $arg1
	$myVar2 = $arg2

; Declare a user-defined function that returns a value
function @myFunction2($arg1 : number, $arg2 : text) : number
	return $arg1 + size($arg2)

init
	; Call a user-defined function
	@myFunction(1, "Hello")

	; Call a trailing function
	$myVar.@myFunction2("Hey")

	; Add values to an array
	$myArray.append(1)
	$myArray.append(5)
	$myArray.append(0)

	; Sort an array
	$myArray.sort()

	; Iterate over an array
	foreach $myArray ($index, $item)
		$myVar4 &= $index:text & ": " & $item:text & ", "

	; Output to a virtual device (ie: a console at input port 0)
	output.0 ($myVar4)
	output.0 (text("Sum of values in the array: {}", $myArray.sum))
	output.1 ($myArray.0:text)
	output.1 ($myArray.1:text)
	var $index = 2
	output.1 ($myArray.$index:text)
	
	; Repeat a statement three times
	repeat 3 ($i)
		output.1 ($i)

	; for loops
	for 3,8 ($i)
		; value of $i will go from 3 to 8 inclusively
	
```

## Types of developer

1. `User`: The person who is using this language to write a script, typically a player in a game. 
2. `Device`: The implementation defining the capabilities and available functions, typically a specific type of programmable virtual device in a specific game. 

# Syntax

XenonCode is designed with a very basic syntax in mind and a very precise structure. 

- Each statement has to be short and easy to read
- Very little special characters needed
- Less criptic than most other languages
- Indentations define the scope (tabs ONLY, no ambiguity there)
- A single instruction per line
- Array indexing, like most other programming languages, is 0-based but uses the `arr.0` dot notation instead of the typical `arr[0]` subscript notation
- 100% Case-insensitive
- An implementation may define custom "Device" functions, objects and entry points

### Types

XenonCode is a typed language, but only includes two generic types, as well as arrays of either, and implementation-defined objects.  

Generic data types the user can declare: 
* `number`
* `text`

A `number` variable is always a 64-bit floating point internally, but may also act as a boolean when its value is either 1 or 0 (true or false), although any value other than zero is evaluated to true.  

`text` variables contain plain unicode text, altough their maximum size depends on the implementation.  
A text literal is defined within double quotes `" "`.  
To use double quotes characters within the text, you may duplicate them.  
There is no other escape sequence mechanism. A backslash `\` is simply part of the string, and the implementation may decide to use it for escape sequences.  
```xc
var $myText = "Say ""Hello"" to this text"
```

Object types are for use by the implementation and are opaque to the user, meaning their structure is not necessarily defined, however the implementation may make availalbe some member functions for those objects to the user.  

Even though this is a typed language, specifying the type is not needed when it can be automatically deduced by the compiler during initialization. The type should only be specified when there is no initialization.  

All user-defined words must start with a prefix character:  
- `$` for variables
- `@` for functions

### Comments

Comments are lines that start with `;`  
A code statement may also end with a trailing comment
NOTE: `//` is deprecated and should NOT be used.  

# Limitations
This language is designed to potentially be executed Server-side in the context of a multiplayer game, hence for security and performance reasons there are limits to what users can do.

- No pointer nor reference types (except for implementation-defined objects, which must be sanitized by the implementation at runtime)
- The number of instructions per cycle may be limited, upon which an overflow may cause a virtual crash for the user
- Arrays may be limited in size at runtime, upon which an overflow may trigger a virtual crash for the user
- Recursive stack (calling a function recursively) is allowed with a specific syntax and is limited to 16 recursions
- Functions MUST be fully defined BEFORE their use, so the order of definition matters (this is what enforces the previous point)

### Per-Virtual-Computer limitations
These are defined per implementation and may include multiple variants or be customizable by the user

- ROM (max compiled program size as a number of 32-bit words)
- RAM (max number of local, global and tmp variables plus all local and global arrays multiplied by their size)
- STORAGE (max number of storage variables plus all storage arrays multiplied by their size)
- Frequency (max frequency for timer functions and input read)
- Ports (max number of inputs/outputs)
- IPC (max instructions per cycle, one line of code may count as multiple instructions)

### Operation on data
- All functions, including timers, are executed atomically, preventing any data-race
- Function arguments are always passed by copy, a function CANNOT modify the variables placed in its argument list
- Trailing functions DO modify the value of the leading variable
- Variable assignments always copy the value for generic types
- Implementation-defined objects are always passed by reference
- Implementation-defined objects cannot be copied unless the implementation provides that functionality through a device function
- Divisions by zero results in a runtime error. It is at the responsibility of the user to make sure to account for it.

### Basic rules
- Variables may be declared using `var` and optionally assigned an initial value otherwise the generic default is used (0 for number and "" for text)
- Object variables must be assigned upon declaration using a constructor or a device function that returns an object of that type
- Starting a statement with a varialbe name (starting with `$`) means that we are modifying its value
- If the next word following a variable assignment is `=`, then the following expression's result is going to be assigned as its value
- If the next word following a variable assignment is a dot and a function, this function call is considered a trailing function
- Calling a trailing function DOES modify the value of the leading variable wich is also used as the function's first argument which must be omitted from the argument list when calling it
- Starting a statement with a function name (starting with `@` for user-defined functions) means that we are calling this function and discarding its return value
- Calling a function will NEVER modify the value of any of its generic-typed arguments passed within parenthesis, generic types are always passed by copy
- Anything in parenthesis that is not preceeded by a function name is considered a separate expression, inner left-most expressions are computed first
- Typical rules apply for mathematical precedence of operators

# Valid usage

XenonCode is designed to be compiled as byteCode which is very fast to parse by the host game at runtime.  

## Each line must begin with one of the following first words (with examples):

### Global scope
- `include` Imports the content of another file
- `const` declare a global constant
- `var` declare a global variable
- `array` declare a global array
- `storage` declare a storage variable or array, which will be persistent across power cycles
- `init` Define the body of the init function, which will execute once when the device is powered on
- `tick` declare the body of the tick function that will execute at each clock cycle
- `function` declare a user-defined function
- `recursive function` declare a user-defined function that may call itself recursively
- `timer` Define the body of a function that will execute repeatedly at a specific frequency in Hz
- `input` Define an input function
- `;` Comments
- an entry point defined by the implementation
- one or more tabs, meaning we're within a function body, then the following rules apply:

### Function body / Entry point
- `var` Declare a new variable in this local scope (not accessible from outside of this function)
- `array` Declare a new array in this local scope (not accessible from outside of this function)
- `$` To assign a new value to an existing variable
- `@` To call a user-defined function
- `output` Built-in function to send data to another device through a specific port
- `foreach` loops through all items of an array
- `repeat` loops a block of code n times
- `while` loops as long as a condition evaluates to true
- `for` loops a block of code, given a first and last index
- `break` stops a loop here as if it completed all iterations
- `continue` stops this iteration of a loop here and run the next iteration immediately
- `if` runs a block of code if a condition evaluates to true
- `elseif` after a if, when the initial condition so far evaluates to false and a new condition evaluates to true
- `else` after a if, when all conditions evaluated to false
- `return` stop the execution of this function here and return a value to the caller

## Reserved keywords
Since all user-defined words must start with either `$` or `@`, there is no need for reserved words.  
The implementation/device must take care of only defining function names and object types that do not conflict with built-in keywords for the version of XenonCode that they're using.  

## Declaring a constant
Constants are named values that should never change throughout the execution of a program. They are fixed values defined at compile-time.  
Their assigned values must be explicitely given and must be expressions which the result can be determined at compile-time.  

`const $stuff = 5` declares a constant named $stuff with the number 5 as its value
`const $stuff = "hello"` declares a constant named $stuff with the text "hello" as its value

## Declaring a variable
Variables are named values that can change throughout the execution of a program.  
We may or may not assign them an initial value.  
If we do not assign an initial value, the default generic value is used, and we must provide a type.  
A variable is only accessible within the scope it has been declared in. For instance, if we declare a variable at the biginning of a function, it is available throughout the entire function. If we declare a variable within a if block, it is only available within that block, until the corresponding `else`, `elseif` or until going back to the parent scope.  
A variable declared in the global scope is accessible from everywhere.  
For variables declared in the global scope, when we assign it an initial value, the given expression must be determined at compile-time.  
Variable names must start with a letter or underscore (`a`-`z` or `_`) then must contain only alphanumeric characters and underscores.  

`var $stuff = 5` declares a number variable with an initial value set to 5 when the program starts  
`var $stuff = "hello"` declares a text variable with an initial value set to "hello" when the program starts  
`var $stuff:number` declares a number variable with an initial value set to 0 when the program starts  
`var $stuff:text` declares a text variable with an initial value set to "" when the program starts  
`var $stuff = position()` declares an instance of an implementation-defined object of the type `position` (this is an example)  

Implementation-defined objects cannot be declared without initialization, since they do not have a default value.  

## Assigning a new value to a variable
To assign a new value to a variable, we can simply start a statement with the variable name followed by a `=` and an expression the result of which will be the new value.  
We may also use a trailing function which will inherently modify the value of said variable.  

`$stuff = 5` assign the value 5 to the variable named $stuff  
`$stuff = $other + 5` assign the result of the expression ($other + 5) to the variable named $stuff  
`$stuff.round()` call a trailing function that rounds the value of the variable  

## Declaring an array
An array is a dynamic list of values of a certain type. We can append or erase values, we can access a specific value from the list, or loop through all its values.  
When declaring an array, we cannot specify an initial value, and we must provide a type.  
Arrays are initialized with zero size when the program starts, values may be added/erased/modified throughout the execution of the program  

`array $stuff:number` declare an array of numbers  
`array $stuff:text` declare an array of texts  

Arrays cannot contain implementation-defined objects, just generic types.  

## Declaring storage
Storage is used to keep some data persistent across power cycles and even through a re-compile.  
We can declare storage variables and arrays of either number or text.  
Storage should ONLY be declared in the global scope.  
```xc
storage var $stuff:number
storage var $stuff:text
storage array $stuff:number
storage array $stuff:text
```

## Accessing/Assigning the nth item within an array
To access or modify the value of a specific item in an array, we must use the trail operator `.` followed by the 0-based index of the item or a variable containing that index  
`$stuff.0 = 5` Assign the value 5 to the first item of the array  
`$stuff.$index = 5` Assign the value 5 to the item with an index defined by the value of $index  
`$value = $stuff.1` Assign the value of the second item of the array to the variable $value  

## Accessing/Assigning the nth character within a text variable
Text variables work in a very similar way to arrays. We can use the trail operator `.` to access or modify the value of specific charaters within a text.  
`$myText.0 = "a"` Set "a" as the first character of $myText  

### Key-Value objects

XenonCode supports its own key-value type that is always stored as text.  
Simply declare a text variable and assign/read its members using its key as the trailing member
```xc
var $myObject = ".a{5}.b{8}" ; you can use the serialization format like so, but you don't have to, you may simply start with an empty text and assign the members one by one
print($myObject.a) ; will print 5
$myObject.b += 2 ; adds 2 to b which was 8 and will now be 10
```

## The Init function
The Init function's body will be executed first everytime the virtual computer is powered on.  
The init function cannot be called by the user. It can only be defined, and the device will automatically call it upon virtual startup.  
```xc
init
    $stuff = 5
    @func1()
    ;...
```

## Tick function
The tick function is executed at the beginning of every clock cycle of this virtual computer.  
The tick function cannot be called by the user. It can only be defined, and the device will automatically call it for each cycle.  
```xc
tick
    ; This body is executed once per clock cycle at the virtual computer's frequency
```

## Timer functions
Timer functions are executed at a specified interval or frequency, but at most once per clock cycle.  
We can either specify an `interval` as in every N seconds or a `frequency` as in N times per second.  
Timer functions cannot be called by the user. They can only be defined, and the device will automatically call them at their appropriate time.  
```xc
timer frequency 4
    ; stuff here runs 4 times per second
timer interval 2
    ; stuff here runs once every 2 seconds
```
Note: If the clock speed of the virtual computer is slower than the given interval or frequency, that timer function will not correctly run at the specified interval or frequency, and may be executed at every clock cycle instead.  

## Input functions
Input functions are a way of accessing the information that we have received from another device.  
They may be executed any number of times per clock cycle, depending on how much data it has received since the previous clock cycle. The implementation may decide to only run it once per cycle using only the latest data received.  
Devices may have an upper limit in the receiving buffer which defines the maximum number of times the input function may be called per clock cycle.  
If that limit has been reached, only the last N values will be kept in the buffer.  
Input functions are like user-defined functions, containing arguments, but no return value, and also we must specify a port index.  
The port index must be specified after the input keyword and a trail operator `.`  
The port index may be specified via a constant as well (must be known at compile time).  
Function arguments must be surrounded with parenthesis and their types must be specified.  
Input functions cannot be called directly by the user. They can only be defined, then the device will automatically call them if data has been received, at the end of a clock cycle.  
```xc
input.0 ($arg1:number, $arg2:text)
    $stuff = $arg1
input.$myPortIndex ($arg1:number, $arg2:text)
    $stuff = $arg1
```

## Output
The output function is how we send data to another device. This function is meant to be called as a statement, and cannot be used in the global scope like the input functions are.  
We must also pass in the port index as we do with the input function, and it can also be specified via a constant that is known at compile-time.  
We must pass a list of arguments surrounded with parenthesis (or an empty set of parenthesis).  
`output.0 ($stuff, $moreStuff)`

## If Elseif Else
Like most programming languages, we can use conditionals.  
```xc
if $stuff == 5
    ; then run this
elseif $stuff == 6
    ; then run that instead
else
    ; all previous conditions evaluate to false, then run this instead
```

## Foreach loops
This loops through all items of an array.  
The block of code under that loop statement will be executed for every item in the array, one by one.  
```xc
foreach $stuff ($index, $item)
    ; we loop through the array $stuff, and we define $index which contains the 0-based index of this item and $item for the current item's value
    ; note that $item is a copy of its value, so modifying the value of $item will not affect the original array $stuff
    ; if we want to persist the modified $item value into the original array, we can use $index to index the element from the array like so:
    $stuff.$index = $item
    ; CAUTION: $index is a reference used internally for the loop, don't modify its value unless you actually want to affect the loop
```
You may also use the foreach loop with key-value objects
```xc
foreach $obj ($key, $value)
    print($key)
    print($value)
```

## Repeat loops
This loop will repeat the execution of the following block a given number of times.  
```xc
repeat 5 ($i)
    ; this block will be repeated 5 times, and $i is the 0-based index of this iteration (first time will be 0, last will be 4)
    ; CAUTION: $i is a reference used internally for the loop, don't modify its value unless you actually want to affect the loop
```
The number of times (above specified as 5) may also be specified via a variable or a constant, but not an expression

## For loops
This loop is similar to repeat but takes in a first and last index instead of a number of repeats
```xc
for 3,8 ($i)
	; $i will start at 3, and end with 8, inclusively, for a total of 6
```

## While loops
This loop will run the following block as long as the given condition evaluates to true.  
```xc
while $stuff < 5
    $stuff++
```

## Break
This keyword is used to stop a loop as if it completed all its iterations.  
```xc
while $stuff < 5
    $stuff++
    if $stuff == 3
        break
```

## Continue
This keyword is used to stop this iteration of a loop here and run the next iteration immediately.  
```xc
while $stuff < 5
    $stuff++
    if $stuff == 2
        continue
```

## Math Operators
- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division
- `%` modulus
- `^` power

## Trailing Operators
- `++` increment the variable's value
- `--` decrement the variable's value
- `!!` reverses the variable's value (if it's value is `0`, sets it to `1`, otherwise to `0`)

## Assignment operators
- `=` just assign the following value directly

The following operators will compute the appropriate math operation between the leading variable and the following expression, then assign the result back to the leading variable.  
- `+=` addition
- `-=` substraction
- `*=` multiplication
- `/=` division
- `%=` modulus
- `^=` to the power
- `&=` concatenate text

## Conditional Operators
- `==` equal to
- `<>` or `!=` not equal to
- `>` greater than
- `<` lesser than
- `<=` lesser or equal to
- `>=` greater or equal to
- `&&` or `and` conditional AND
- `||` or `or` conditional OR
- `xor` is also available, equivalent to (!!a != !!b)

## Other operators
- `.` (trail operator) refer to a sub-item of an array or text or call a trailing function on the leading variable, or a member of an object
- `:` (typecast operator) cast as another type
- `&` (concat operator) concatenate texts
- `!` (not operator) reverses a boolean value or expression (non-zero numbers become 0, and 0 becomes 1)

## Casting (parse variables as another type)

To parse an existing variable as another type, simply add a colon and the type, like so: 
```xc
$someTextValue = $someNumberValue:text
```
This only works with generic types `number` and `text`, not arrays or objects.  

## String concatenation

To concatenate texts, simply separate all text values/variables with the concat operator `&` in the same assignment (don't forget to cast to text if you need to).  
```xc
$someTextVar = "Hello, " & $someName & "!, My age is " & $age:text & " and I have all my teeth"
```

## Include
You may want to split your project into multiple source files.  
To do this, you can put some code into another `.xc` file and use the `include` keyword in the parent file. 
```xc
include "test.xc"
```
This is effectively the same as taking all the lines from `test.xc` and inserting them into the current file where the `include` is.  
This can be done on multiple levels, just make sure that a file does not include itself, directly or indirectly, in which case the definitions within it will conflict and result in a compile error.

# User-Defined Functions

Functions define a group of operations that we may call one or more times during the execution of the program. 

The operations to run in a function appear within its body. 

Functions may have arguments that can be used within the body so that the operations may have a variation depending on the value of some variables. 

Function arguments are defined after the function name, within parenthesis and they can be of type `number`, `text`, or implementation-defined object. 

Function names have the same rules as variable names.  

NOTE: Functions MUST be fully defined before their use. This means that the order of function declaration matters and we can only call a function that has been declared ABOVE the caller and a function CANNOT call itself. This enforces the "no-stack-recursion" rule.  

### Declaration

Here are some function declaration examples  

This function takes in a single number argument:
```xc
function @func0 ($var1:number)
```

This function takes two number arguments:
```xc
function @func1 ($var1:number, $var2:number)
```

This function takes a number argument and a text argument:
```xc
function @func2 ($var1:number, $var2:text)
```

This function takes an implementation-defined object type `position` argument:
```xc
function @func3 ($var1:position)
```

This function takes a number argument and a text argument and returns a number value:
```xc
function @func2 ($var1:number, $var2:text) : number
```

### Body
The body of a function (the operations to be executed when calling it) must be on the following lines after the declaration, indented with one tab.  
Empty lines within a body are allowed and ignored by the compiler.  
Functions bodies may have a `return` statement optionally followed by an expression that would be used to assign a leading variable in the caller statement.  
When returning a value, the return type must be provided at the end of the arguments, after the closing parenthesis and a colon.  

```xc
function @func1 ($var1:number, $var2:number) : number
    return $var1 + $var2
```

### Call

Calling a function will run the operation in its body. 
To call a function, simply write the name of the function (starting with `@` for user-defined functions), then its arguments within parenthesis separated by commas, like so:  
```xc
@func1(4, 6)
```
Here we have passed two number arguments, thus this call executes the body of the declaration above.  
It is of course also possible to use variables or even complex expressions instead of just literal numbers as the function arguments.  

#### NOTE:
Omitted arguments are legal.  
Their value initially takes the default empty ("" or 0) then persists with what is passed in or assigned to them for the following calls to that function.  
Changing the value of an argument within that function will also be persistent for the next call to that function, if that argument is omitted. 
Hence, if a concept of a default argument value is needed, they can be assigned to the argument in the body of that function after their use.  
This omitted argument concept can also be thought of as a concept similar to static local variables in C++.  

### Return value

Functions may return a value using the `return` keyword.  
This returned value may be assigned to a variable like so : 
```xc
$value = @func1(4, 6)
```

# Recursive functions
A recursive function is a function that calls itself.
To define a recursive function, use `recursive function` and `recurse` to call the function itself.
```xc
recursive function @recursiveFunc($myVar:number)
    if $myVar < 15
        recurse($myVar++)
```
This example of a recursive function will call itself as long as the value of $myVar is less than 15.
Recursive functions are limited to 16 recursive calls.

Note: The `recurse` keyword is only available within recursive functions and is used to call the function itself. The argument list must be the same as the function definition.

# Trailing functions
Any function may be called as a trailing function, even user-defined functions.  
The way they work is that under the hood the leading variable is passed as the first argument to that function, and then assigned the returning value.  
When calling a trailing function, we must ommit the first argument as it automatically sends the leading variable as its first argument under the hood.  
If the function definition does not have any arguments, this is still valid, although we simply don't care about the current value of the leading variable, but we'll assign a new value to it.  
The function definition MUST have a return type that matches that of the leading variable, if it's a generic type.  
A trailing function may be called on implementation-defined objects, in which case the first argument must be of that object type, there is no return type in the function and it must NOT return any value.  
Since we cannot pass Arrays as function arguments, arrays can only take their own specifically defined trailing functions.  
```xc
$myVariable.round()
```
```xc
$myVariable.@func1(6)
```
```xc
$myArray.clear()
```

# Built-in functions

### Math
These functions are defined in the base language and they take one or more arguments.  
Trailing math functions will use the leading variable as its first argument and modify that variable by assigning it the return value.  
- `floor`(number)
- `ceil`(number)
- `round`(number)
- `sin`(number)
- `cos`(number)
- `tan`(number)
- `asin`(number)
- `acos`(number)
- `atan`(number) or (number, number)
- `abs`(number)
- `fract`(number)
- `log`(number, base)
- `sqrt`(number)
- `sign`(number [, default])
- `pow`(number, exponent)
- `clamp`(number, minimum, maximum)
- `step`(edge1, edge2, number) or (edge, number)
- `smoothstep`(edge1, edge2, number)
- `lerp`(a, b, number)
- `mod`(number, divisor) the modulus operator
- `min`(number, number)
- `max`(number, number)
- `avg`(number, number)
- `add`(number, number)
- `sub`(number, number)
- `mul`(number, number)
- `div`(number, number)

### Text functions
- `substring`(inputText, start, length) returns a new string
- `text`(inputTextWithFormatting, vars ...)
- `size`($myText) returns the number of characters in $myText
- `last`($myText) returns the last character in $myText
- `lower`($myText) returns the lowercase version of $myText
- `upper`($myText) returns the uppercase version of $myText

#### Formatting
The `text` function takes a format as the first argument.  
The format is basically a text that may contain enclosing braces that will be replaced by the value of some variables or expressions.  
Exemple: 
```xc
$formattedText = text("My name is {} and I am {} years old.", $name, $age)
```
Empty braces above will be replaced by the corresponding variables in the following arguments in the same order.  
It is also possible to format number variables in a specific way by providing some pseudo-values within the braces like so:  
- `{}` automatically display only the necessary digits based on the value (ex: `3` or `123.456`)
- `{0}` round to nearest integer value (ex: `3` or `123`)
- `{00}` round to nearest integer value but also display at least two digits (ex: `03` or `123`)
- `{0e}` display the rounded integral value as a scientific notation (ex: `3e+00` or `1e+02`)
- `{0e.00}` display the value as a scientific notation with two digits after the decimal (ex: `3.00e+00` or `1.23e+02`)
- `{0.0}` round to one digit after the decimal (ex: `3.0` or `123.5`)
- `{0000.00}` display at least 4 integral digits and round to two digit after the decimal (ex: `0003.00` or `0123.46`)

### Trailing functions for Arrays
These functions MUST be called as trailing functions, and they do not return anything, instead they modify the array
- $myArray.`clear`() Empty an array
- $myArray.`sort`() Sort an array in Ascending order
- $myArray.`sortd`() Sort an array in Descending order
- $myArray.`append`(value1, value2, ...) Append one or more values to the end of an array.
- $myArray.`pop`() Erase the last value in an array, reducing its size by one
- $myArray.`insert`(index, value) Insert a new value to an array at a specific position, pushing back all following values by one
- $myArray.`erase`(index) Erase an element from an array at a specific index, pulling back all following values by one
- $myArray.`fill`(qty, value) Resize and Fill an array with a given size and the specified value for all items (this clears any values previously present in the array)
- $myArray.`from`(other [, separator]) Set the contents of the array to another array or text. Separator is for splitting with a specific string (only valid when other is a text). This function also works in reverse when executed on a text given an array and a separator.

### Trailing members for Arrays
Using the trail operator `.`, we can also return a specific information about certain types of variables.  
- $myArray.`size` returns the number of elements in $myArray
- $myArray.`min` returns the minimum value within a number array
- $myArray.`max` returns the maximum value within a number array
- $myArray.`avg` returns the average value within a number array
- $myArray.`med` returns the median value within a number array
- $myArray.`sum` returns the sum of all values within a number array
- $myArray.`last` returns the value of the last item within an array, this also allows to modify that value by assigning an expression

### Other useful helpers
- `contains`($myText, "str") returns 1 if $myText contains "str", otherwise 0
- `find`($myText, "str") returns the index of the first character of the first occurance of "str" in $myText, otherwise -1 if not found
These also work on arrays.  
- `isnumeric`($myText) returns 1 if $myText is a number, otherwise 0

### Ternary operator
The ternary operator is a shorthand conditional expression that returns a value depending on the evaluation of a condition.
```xc
$myVar = if($cond, $valTrue, $valFalse)
```

### Device functions
An implementation should define application-specific device functions.  
Here are examples of basic device functions that MAY or MAY NOT be defined:  
- `delta`() returns the time difference in seconds from the last execution of this `delta` function  
Device functions that do not require any arguments may be used without parenthesis when called from within an expression.  

# Compiler Specifications
This section is intented for game developers who want to use this in their game.  

XenonCode comes with its own parser/compiler/interpreter library, and a very simple cli tool.  

## Editor
It is encouraged that the code editor runs the following parse on the current line on each keystroke: 
- Replace leading sequences of spaces with exactly one tab more than the previous non-empty line, if it starts further than it
- Autocomplete words upon pressing the space bar if that word is not an existing symbol
    - Write the minimum remaining characters (common denominator) of the symbols that start with the written characters
        - If this is the leading word: Only autocomplete with global scope statement words
        - If preceded with tabs: Only autocomplete with function body statement words
        - When the first character in the current word is a `$`, autocomplete with a user variable
        - When the first character in the current word is a `@`, autocomplete with a user function
        - Otherwise, Autocomplete with one of:
            - Current function/foreach/repeat arguments
            - Built-in functions
            - Operators

## Runtime

Upon powering up the virtual computer:
- Execute the body of the init() function  

One clock cycle, executed 'frequency' times per second:
- Execute all input functions that have received some data since the last cycle (once each per cycle)
- Execute custom events / entry points
- Execute the tick function
- Execute all timer functions sequentially if their time is due
- Sleep for Elapsed-1/Frequency Seconds

# Testing XenonCode
You may want to test XenonCode or practice your coding skills.  
There is an online fiddle tool at <a href="https://xenoncode.com/">XenonCode.com</a>  

Otherwise, you may want to try running it directly on your computer.  
For this, XenonCode has a cli with a `-run` command to test some scripts in the console.  
This repository comes with the cli tool, located in `build/xenoncode`  
Here's how you can download and run XenonCode:
```shell
# Clone this github repository
git clone https://github.com/batcholi/XenonCode.git
cd XenonCode
# Compile & Run the XC program located in test/
build/xenoncode -compile test -run test
```
You may edit the .xc source files in `test/` then try running the last line again to compile & run.  
`test/storage/` directory will be created, it will contain the storage data (variables prefixed with the `storage` keyword).  
Note that this `-run` command is meant to quickly test the language and will only run the `init` function.  
Also, make sure that your editor is configured to use tabs and not spaces, for correct parsing of indentation.  

If you want to integrate XenonCode into your C++ project, you can include `XenonCode.hpp`.  
Further documentation will be coming soon for this, in the meantime you may use `main.cpp` as an example but its usage is still subject to change.  
