# XenonCode - Documentation

**XenonCode** is a lightweight programming language designed as high level scripting within games for virtual computing devices (ie: a programmable computer within a game).

# Capabilities

- Typed Variables, Constants and Dynamic Arrays
- Standard arithmetic operations on numeric values
- Easy string concatenation and formatting
- User-defined functions
- Device-defined functions
- Trailing functions support
- Built-in standard math functions
- Built-in IO operations between virtual devices
- Synchronized interval functions (timers)
- `if`/`elseif`/`else` conditionals
- `while` loops
- `foreach` loops for arrays
- `repeat n` loops (akin to `for (i=0;i<n;i++)` in most other languages)

# Syntax

XenonCode is designed with a very basic syntax in mind and a very precise structure. 

- Each statement has to be short and easy to read
- Very little special characters needed
- Indentations define the scope (tabs)
- A single instruction per line
- Array indexing is 1-based, NOT 0-based, and uses a `arr.1` notation instead of `arr[0]` to make it easier to adopt
- 100% Case-insensitive

### Types

XenonCode is a typed language, but only includes two generic types, as well as arrays of either, and special opaque data types.  

Generic data types the user can declare: 
* `number`
* `text`

A `number` variable is always a 64-bit floating point internally, but may also act as a boolean when its value is either 1 or 0 (true or false), although any value other than zero is considered true.  

`text` variables contain pure arbitrary text, altough their maximum size depends on the implementation.  

Special `data` types are for use by the implementation and are opaque to the user, meaning their structure is not specifically defined, and cannot be declared by a user. They can only be passed around from one function call to another. 

Even though this is a typed language, specifying the type is not needed when it can be automatically deduced by the compiler.  

All user-defined words must start with a prefix character:  
- `$` for variables
- `@` for functions

### Comments

Comments are lines that start with either `;` or `//`  
A code statement may also end with a trailing comment

# Limitations
This language is designed to potentially be executed on Server-side in a multiplayer game, hence for security and performance reasons there are limits to what users can do.

- No pointer nor reference types
- The number of instructions per cycle may be limited, on which an overflow can slow things down
- Arrays may be limited in size at runtime, on which an overflow may trigger a virtual crash
- Recursive stack (calling a function recursively) is NOT allowed

### Per-Virtual-Computer limitations
These are defined per implementation and may include multiple variants or be customizable by the user

- ROM (max compiled program size as a number of 32-bit words)
- RAM (max number of local, global and tmp variables plus all local and global arrays multiplied by their size)
- STORAGE (max number of storage variables plus all storage arrays multiplied by their size)
- Frequency (max frequency for timer functions and input read)
- Ports (max number of inputs/outputs)
- IPC (max instructions per cycle, one line of code may contain multiple instructions)

### Operation on data
- All functions, including timers, are executed atomically, preventing any data-race
- Function arguments are always passed by copy, a function cannot modify the variables placed in its argument list
- Trailing functions DO modify the value of the leading variable
- Variable assignments always copy the value
- Divisions by zero result in the value zero. It is at the responsibility of the user to make sure to account for it.

### Basic rules
- Variables may be declared using `var` and optionally assigned an initial value otherwise the generic default is used (0 for number and "" for text)
- Starting a statement with a varialbe name (starting with `$`) means that we are modifying its value
- If the next word following a variable assignment is `=`, then the following expression's result is going to be assigned as its value
- If the next word following a variable assignment is a dot and a function, this function call is considered a trailing function
- Calling a trailing function DOES modify the value of the leading variable wich is also used as the function's first argument which must be omitted from the argument list
- Starting a statement with a function name (starting with `@` for user-defined functions) means that we are calling this function
- Calling a function will NEVER modify the value of any of its arguments passed within parenthesis
- Anything in parenthesis that is not preceeded by a function name is considered a separate expression, inner left-most expressions are computed first

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
- `timer` Define the body of a function that will execute repeatedly at a specific frequency in Hz
- `input` Define an input function
- `;` or `//` Comments
- one or more tabs, meaning we're within a function body, then the following rules apply:

### Function body
- `var` Declare a new variable in this local scope
- `array` Declare a new array in this local scope
- `$` To assign a new value to an existing variable
- `@` To call a leading user-defined function
- `output` Built-in function to send data to another device through a specific port
- `foreach` Loops through all items of an array
- `repeat` loops a block of code n times
- `while` loops as long as a condition evaluates to true
- `break` stops a loop here as if it completed all iterations
- `loop` stops this iteration of a loop here and run the next iteration immediately
- `if` runs a block of code if a condition evaluates to true
- `elseif` after a if, when the initial condition so far evaluates to false and a new condition evaluates to true
- `else` after a if, when all conditions evaluated to false
- `return` stop the execution of this function here and return a value to the caller

## Reserved keywords
Since all user-defined words must start with either `$` or `@`, there is no need for reserved words.

## Declaring a constant
Constants are named values that should never change throughout the execution of a program. They are fixed values defined at compile-time.  
Their assigned values must be explicitely given and must be expressions which the result can be determined at compile-time.  

`const $stuff = 5` // declares a constant named $stuff with the number 5 as its value
`const $stuff = "hello"` // declares a constant named $stuff with the text "hello" as its value

## Declaring a variable
Variables are named values that can change throughout the execution of a program.  
We may or may not assign them an initial value.  
If we do not assign an initial value, the default generic value is used, and we must provide a type.  
A variable is only accessible within the scope it has been declared in. For instance, if we declare a variable at the biginning of a function, it is available throughout the entire function. If we declare a variable within a if block, it is only available within that block, until an Else, Elseif or going back to the parent scope.  
A variable declared in the global scope is accessible from everywhere.  
For variables declared in the global scope, when we assign it an initial value, the given expression must be determined at compile-time.  
Variable names must start with a letter or underscore (`a`-`z` or `_`) then must contain only alphanumeric characters and underscores.  

`var $stuff = 5` // declares a number variable with an initial value set to 5 when the program starts
`var $stuff = "hello"` // declares a text variable with an initial value set to "hello" when the program starts
`var $stuff:number` // declares a number variable with an initial value set to 0 when the program starts
`var $stuff:text` // declares a text variable with an initial value set to "" when the program starts

## Assigning a new value to a variable
To assign a new value to a variable, we can simply start a statement with the variable name followed by a `=` and an expression the result of which will be the new value.  
We may also use a trailing function which will inherently modify the value of said variable.  

`$stuff = 5` // assign the value 5 to the variable named $stuff
`$stuff = $other + 5` // assign the result of the expression ($other + 5) to the variable named $stuff
`$stuff.round()` // call a trailing function that rounds the value of the variable

## Declaring an array
An array is a dynamic list of values of a certain type. We can append or erase values, we can access a specific value from the list, or loop through all its values.  
When declaring an array, we cannot specify an initial value, and we must provide a type.  
Arrays are initialized with zero size when the program starts, values may be added/erased/modified throughout the execution of the program  

`array $stuff:number` // declare an array of numbers
`array $stuff:text` // declare an array of texts

## Declaring storage
Storage is used to keep some data persistent across power cycles and even through a re-compile.  
We can declare storage variables and arrays of either number or text.  
Storage must be declared in the global scope.  
We may assign initial values to a storage.  
```
storage var $stuff = 5
storage var $stuff:text
storage array $stuff:numeric
```

## Accessing/Assigning the nth item within an array
To access or modify the value of a specific item in an array, we must use the trail operator `.` followed by the 1-based index of the item or a variable containing that index
`$stuff.1 = 5` // Assign the value 5 to the first item of the array
`$stuff.$index = 5` // Assign the value 5 to the item with an index defined by the value of $index
`$value = $stuff.2` // Assign the value of the second item of the array to the variable $value

## Accessing/Assigning the nth character within a text variable
Text variables work in a very similar with to arrays. We can use the trail operator `.` to access or modify the value of specific charaters within a text.  
`$myText.1 = "a"` // Set "a" as the first character of $myText

## The Init function
The Init function's body will be executed first everytime the virtual computer is powered on.  
The init function cannot be called by the user. If can only be defined.  
```
init
    $stuff = 5
    @func1()
    //...
```

## Tick function
The tick function is executed at the beginning of every clock cycle of this virtual computer.  
The tick function cannot be called by the user. If can only be defined.  
```
tick
    // This body is executed once per clock cycle at the virtual computer's frequency
```

## Timer functions
Timer functions are executed at a specified interval or frequency, but at most Once per clock cycle.  
We can either specify an `interval` as in every N seconds or a `frequency` as in N times per second.  
Timer functions cannot be called by the user. They can only be defined.  
```
timer frequency 4
    // stuff here runs 4 times per second
timer interval 2
    // stuff here runs once every 2 seconds
```
Note: If the clock speed of the virtual computer is slower than the given interval or frequency, that timer function will not correctly run at the specified interval or frequency, and may be executed at every clock cycle instead.  

## Input functions
Input functions are a way of accessing the information that we have received from another device.  
They may be executed any number of times per clock cycle, depending on how much data it has received since the previous clock cycle.  
Devices may have an upper limit in the receiving buffer which defines the maximum number of times the input function may be called per clock cycle.  
If that limit has been reached, only the last N values will be kept in the buffer.  
Input functions are like a user-defined function, containing arguments, but no return value, and also we must specify a port index.  
The 1-based port index must be specified after the input keyword and a trail operator `.`  
The port index may be specified via a constant as well.  
Function arguments must be surrounded with parenthesis and their types must be specified. We may define multiple input functions using the same port as long as their argument types/count are not the same.  
Input functions cannot be called directly by the user. They can only be defined.  
```
input.1 ($arg1:number, $arg2:text)
    $stuff = $arg1
input.$myPortIndex ($arg1:number, $arg2:text)
    $stuff = $arg1
```

## Output
The output function is how we send data to another device. This function is meant to be called in a statement, and cannot be defined in the global scope like the input functions are.  
We must also pass in the port index as we do with the input function, and it can also be specified via a constant.  
We must pass a list of arguments surrounded with parenthesis (or an empty set of parenthesis) and the matching input function on the other device will be executed.  
`output.1 ($stuff, $moreStuff)`

## If Elseif Else
Like most programming languages, we can use conditionals.  
```
if $stuff == 5
    // then run this
elseif $stuff == 6
    // then run that instead
else
    // all previous conditions evaluate to false, then run this instead
```

## Foreach loops
This loops through all items of an array.  
The block of code under that loop statement will be executed for every item in the array, one by one.  
```
foreach $stuff ($item)
    // we loop through the array $stuff, and we define $item and its value is the current item's
foreach $stuff ($item, $i)
    // here we also define $i which contains the 1-based index of this item within the array $stuff
```

## Repeat loops
This loop will repeat the execution of the following block a given number of times.  
```
repeat 5 ($i)
    // this block will be repeated 5 times, and $i is the 1-based index of this iteration (first time will be 1, last will be 5)
```
The number of times (above specified as 5) may also be specified via a variable or a constant, but not an expression

## While loops
This loop will run the following block as long as the given condition evaluates to true.  
```
while $stuff < 5
    $stuff++
```

## Break
This keyword is used to stop a loop as if it completed all its iterations.  
```
while $stuff < 5
    $stuff++
    if $stuff == 5
        break
```

## Loop
This keyword is used to stop this iteration of a loop here and run the next iteration immediately
```
while $stuff < 5
    $stuff++
    if $stuff == 2
        loop
```

## Math Operators
- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division
- `%` modulo
- `^` power

## Trailing Operators
- `++` increment the variable's value
- `--` decrement the variable's value
- `!!` reverses the variable's value

## Assignment operators
These operators will compute the appropriate math operation and assign the result to the leading variable.  
- `+=`
- `-=`
- `*=`
- `/=`
- `%=`
- `^=`
- `&=`

## Conditional Operators
- `==` equal to
- `<>` or `!=` not equal to
- `>` greater than
- `<` lesser than
- `<=` lesser or equal to
- `>=` greater or equal to
- `&&` or `and` conditional AND
- `||` or `or` conditional OR

## Other operators
- `.` (trail operator) refer to a sub-item of an array (or of a built-in function) or call a trailing function on the leading variable
- `:` (typecast operator) cast as another type
- `&` (concat operator) concatenate texts
- `!` (not operator) reverses a boolean value or expression (non-zero numbers become 0, and 0 becomes 1)

## Casting (parse variables as another type)

To parse an existing variable as another type, simply add a colon and the type, like so: 
```$someTextValue = $someNumberValue:text```
This only works with simple variable types `number` and `text`, not arrays or special data  

## String concatenation

To concatenate texts, simply separate all text values/variables with the concat operator `&` in the same assignment (don't forget to cast to text if you need to).  
```$someTextVar = "Hello, " & $someName & "!, My age is " & $age:text & " and I have all my teeth"```

# User-Defined Functions

Functions define a group of operations that we may call one or more times during the execution of the program. 

The operations to run in a function appear within its body. 

Functions may have arguments that can be used within the body so that the operations may have a variation depending on the value of some variables. 

Function arguments are defined after the function name, within parenthesis and they can be of type `number`, `text`, or a special `data` type. 

Function names have the same rules as variable names.  

### Declaration

Here are some function declaration examples  

This function takes in a single number argument:
```function @func0 ($var1:number)```

This function takes two number arguments:
```function @func1 ($var1:number, $var2:number)```

This function takes a number argument and a text argument:
```function @func2 ($var1:number, $var2:text)```

This function takes a special data argument:
```function @func3 ($var1:data)```

### Body
The body of a function (the operations to be executed when calling it) must be on the following lines after the declaration, indented with one tab.  
Empty lines within a body are allowed and ignored by the compiler.  
Functions bodies may have a `return` statement optionally followed by an expression that would be used to assign a leading variable in the caller statement.  
When returning a value, the return type must be provided at the end of the arguments, after the closing parenthesis and a colon.  

```
function @func1 ($var1:number, $var2:number) : number
    return $var1 + $var2
```

### Call

Calling a function will run the operation in its body. 
To call a function, simply write the name of the function starting with `@`, then its arguments within parenthesis and separated by a comma, like so:  
```@func1(4, 6)```
Here we have passed two number arguments, thus this call execute the body of the second declaration above.  
It is of course also possible to use variables instead of just literal numbers as the function arguments.  

### Return value

Functions may return a value using the `return` keyword.  
This returned value may be assigned to a variable like so : 
```$value = @func1(4, 6)```

# Trailing functions
Any function may be called as a trailing function, even user-defined functions.  
The way this works is that it uses the leading variable as the first argument, and assigns the returning value to that leading variable.  
When calling a trailing function, we must ommit the first argument as it already uses the leading variable as its first argument.  
If the function definition does not have any arguments, this is still valid, although we simply don't care about the current value of the leading variable, but we'll assign a new value to it.  
If the function definition does not have a return type, the value of the leading variable may be assigned the default generic value of 0 or "".  
Since we cannot pass Arrays as function arguments, arrays can only take their own specifically defined trailing functions.  
```$myVariable.round()```
```$myVariable.@func1(6)```

# Built-in functions

### Math
These functions are defined in the base language and they take one or more arguments.  
Trailing math functions will use the leading variable as its first argument and modify that variable with the return value.  
- `floor`(number)
- `ceil`(number)
- `round`(number) // takes an optional second argument to specify the number of decimals, otherwise rounding to integer
- `sin`(number)
- `cos`(number)
- `tan`(number)
- `asin`(number)
- `acos`(number)
- `atan`(number)
- `abs`(number)
- `fract`(number)
- `log`(number, base)
- `sqrt`(number)
- `sign`(number)
- `pow`(number, exponent)
- `clamp`(number, minimum, maximum)
- `step`(edge1, edge2, number) or (edge, number)
- `smoothstep`(edge1, edge2, number)
- `lerp`(a, b, number)
- `mod`(number, divisor) // the modulo operator
- `min`(number, number)
- `max`(number, number)
- `avg`(number, number)
- `add`(number, number)
- `sub`(number, number)
- `mul`(number, number)
- `div`(number, number)

### Text functions
- `substring`(inputText, start, length) // returns a new string
- `text`(inputTextWithFormatting, vars ...) // Formatting specs coming soon

### Trailing functions for Arrays
These functions MUST be called as trailing functions, and they do not return anything, instead they modify the array
- $myArray.`clear`() // Empty an array
- $myArray.`sort`() // Sort an array in Ascending order
- $myArray.`sortd`() // Sort an array in Descending order
- $myArray.`append`(value) // Append a new value to an array
- $myArray.`pop`() // Erase the last value in an array, reducing its size by one
- $myArray.`insert`(index, value) // Insert a new value to an array after a position, pushing back all following values by one
- $myArray.`erase`(index) // Erase an element from an array at a specific index, pulling back all following values by one
- $myArray.`fill`(qty, value) // Resize and Fill an array with a given size and the specified value for all items (this clears any values previously present in the array)

### Trailing members for Arrays and Texts
Using the trail operator `.`, we can also return a specific information about certain types of variables.  
- $myArray.`size` // returns the number of elements in $myArray
- $myText.`size` // returns the number of characters in $myText
- $myArray.`min` // returns the minimum value within a number array
- $myArray.`max` // returns the maximum value within a number array
- $myArray.`avg` // returns the average value within a number array
- $myArray.`med` // returns the median value within a number array
- $myArray.`sum` // returns the sum of all values within a number array
- $myArray.`last` // returns the value of the last item within an array, this also allows to modify that value by assigning an expression
- $myText.`last` // returns the last character of a text variable, this also allows to modify that last character by assigning an expression

### Device functions
An implementation should define application-specific device functions.  
Here are examples of basic device functions:  
- `delta`() // returns the time difference in seconds from the last execution of this function  

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
- Execute the tick function
- Execute all timer functions sequentially if their time is due
- Execute all input functions that have received some data since the last cycle
- Sleep for Elapsed-1/Frequency Seconds

# Testing XenonCode
You may want to test XenonCode or practice your coding skills.  
For this, XenonCode's cli has a `-run` command to test some scripts in the console.  
This repository comes with the cli tool, located in `build/xenoncode`
```shell
# Clone this github repository
git clone https://github.com/batcholi/XenonCode.git
cd XenonCode
# Compile & Run the XC program located in test/
build/xenoncode -compile test -run test
```
You may edit the .xc source files in `test/` then try running the last line again to compile & run.  
`test/storage/` directory will be created, it will contain the storage data (variables prefixed with the `storage` keyword).  
Those are human-readable and can be edited by hand.  
Note that this `-run` command is meant to quickly test the language and will only run the `init` function.  
Also, make sure that your editor is configured to use tabs and not spaces, for correct parsing of indentation.  
