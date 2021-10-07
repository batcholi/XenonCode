# XenonCode - Documentation

**XenonCode** is a lightweight programming language designed as high level scripting within games for virtual computing devices (ie: a programmable computer within a game).

# Capabilities

- Number and Text variables and constants
- Fixed size Arrays defined in the global scope
- Standard arithmetic operations on numeric values
- Easy string concatenation
- User-defined functions with overloadable arguments
- Device-defined functions
- Synchronized interval functions (timers)
- Built-in IO operations for inter-virtual-device data transfer
- Built-in standard math functions
- Trailing functions support
- `if`/`elseif`/`else` conditionals
- `while` loops
- `foreach` loops for arrays
- `repeat n` loops (akin to `for (i=0;i<n;i++)` in most other languages)

# Syntax

XenonCode is designed with a very basic syntax in mind, with a very precise structure. 

- Each statement has to be very short and easy to read
- Very little special characters needed
- No semicolon at the end of each statement
- Indentations define the scope (tabs)
- Forced to have spaces between expressions and operators
- A single statement or instruction per line
- No need to care about the order of operations (reason above)
- Array indexing is 1-based, NOT 0-based, and uses a `arr#1` notation instead of `arr[0]` to make it easier to adopt
- 100% Case-insensitive

### Types

XenonCode is a typed procedural language with overload resolution, but only includes two generic types, as well as arrays of either, and special opaque data types.  

Generic data types the user can declare a variable of: 
* `number`
* `text`

A `number` variable is always a 64-bit floating point internally, but may also act as a boolean when its value is either 1 or 0 (true or false), although any value other than zero is considered true.  

`text` variables contain pure arbitrary text, altough their maximum size depends on the implementation.  

Special `data` types are for use by the implementation and are opaque to the user, meaning their structure is not specifically defined, and cannot be declared by a user. They can only be passed around from one function call to another. They may also support member functions.  

Even though this is a typed language, specifying the type is not needed when it can be automatically deduced by the compiler.  

All user-defined words must start with a prefix character:  
- `$` for variables
- `@` for functions

### Comments

Comments are lines that start with either `;` or `//`  
A code statement may also have a trailing comment

# Limitations
This language is designed to potentially be executed on Server-side in a multiplayer game, hence for security and performance reasons there are limits to what users can do.

- No pointer nor reference types
- Arrays are of fixed size and must be declared in the global scope
- Recursive stacks (calling a function recursively) are NOT allowed
- No enclosed expressions. Every operation must be on separate lines, creating temporary variables if we need to

### Per-Virtual-Computer limitations
These are defined per implementation and may include multiple variants

- ROM (max program size in number of characters)
- RAM (max number of arrays multiplied by their size, plus global variables)
- Timers (max number of timer functions)
- Frequency (max frequency for timer functions and input read)
- Ports (max number of inputs/outputs)
- IPC (max instructions per cycle, one instruction is one line of code)

### Operation on data
- All functions, including timers, are executed atomically, preventing any data-race situation.
- Leading function arguments are always passed by copy
- Trailing functions DO modify the value of the leading variable
- Variable assignations always copy the value

# Valid usage

XenonCode is also designed to be very easy to parse by the host game at runtime.  

### Basic rules
- Starting a statement with a varialbe name (starting with `$`) means that we are setting/modifying its value
- Variables may be declared in the global scope using `var` and optionally assigned an initial value
- Variables may be assigned in a function scope, if it already exists in a parent scope it will use that one, otherwise will create a temporary variable
- If the next word following a variable assignment is `=`, then the following expression's result is going to be assigned as its value
- If the next word following a variable assignment is a system function, this function call is considered a trailing function
- Calling a trailing function DOES modify the value of the leading variable wich is also used as the function's first argument
- Starting a statement with a function name (starting with `@` for user-defined functions) means that we are calling this function, and anything that follows are its arguments
- Calling a leading function will NEVER modify the values of any of its arguments
- If the next word following a `=` in a variable assignemnt is a function name, it is treated the same as a leading function, but its return value is assigned to the leading variable. All remaining expressions are used as arguments for this function.

## Each line must begin with one of the following first words (with examples):

### Global scope
- `init` the body of this function will execute once, when the device is powered on
- `timer` `frequency 10` the body of these functions will execute repeatedly at a specific frequency in Hz
- `input` `1 $var1:number $var2:text` declare an input, its arguments and its body
- `var` `$numberVar1:number` declare a global number variable
- `var` `$numberVar1 4` declare a global number variable with an initial value, type is automatically deduced
- `var` `$textVar1:text "hello"` declare a global text variable with an (optional) initial value
- `const` `$numberConstant1 4` declare a global number constant with its value
- `const` `$textConstant1 "Hello"` declare a global text constant with its value
- `array` `10 $numberArray1:number 0` declare an array of 10 numbers initialized to 0
- `function` `@testFunc $var1:text` declare a function and its body
- one or more tabs, meaning we're within a function body, then the following rules apply:

### Function body
- `$textVar1 = "hello"`
- `$numberVar1 = 4`
- `$numberArray1#4 = 0` set the value of the 4th item of the array to 0 (yes, arrays are 1-based)
- `$tmpNumberVar2 = round $numberVar1` define a local/temporary number variable with a math expression as its value
- `$numberVar1 multiply 2` multiplies $numberVar1 by 2 (using a trailing function)
- `output 2 "test"`
- `@func1 $arg1` using a leading user-defined function, passing it a variable named $arg1 as an argument
- `foreach $item in $numberArray`
    - `break` stops the loop here as if it completed all iterations
    - `next` stops this iteration here and run the next iteration immediately
- `repeat 10 $n`
    - `break`
    - `next`
- `while $condition`
    - `break`
    - `next`
- `if $var1 == 4`
- `elseif $var1 == 5`
- `else`
- `$var1 clear` clears the content of a variable (optionally takes an argument as the clear value)
- `return $numberVar1` stop the execution of this function here and return a value to the caller

## Reserved keywords
Since all user-defined words must start with either `$` or `@`, there is no need for reserved words.

# Math Operators
- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division
- `^` power

# Conditional Operators
- `==` or `is` equal to
- `<>` or `not` not equal to
- `>` or `moreThan` greater than
- `<` or `lessThan` lesser than
- `<=` or `atMost` lesser or equal to
- `>=` or `atLeast` greater or equal to
- `&&` or `and` conditional AND
- `||` or `or` conditional AND
- `xor` conditional exclusive or

## Casting (parse variables as another type)

To parse an existing variable as another type, simply add a colon and the type, like so: 
```$someTextValue = $someNumberValue:text```
This only works with simple variable types `number` and `text`, not arrays or special data

## String concatenation

To concatenate texts, simply compound all text values/variables in the same assignation (don't forget to cast to text if you need to).  
```$someTextVar = "Hello, " $someName "!, My age is " $age:text " and I have all my teeth"```

# Built-in functions

### Device functions

Device functions are defined per implementation. In other words, the games that implement this language into their gameplay may define their own functions here.  

These functions may also be different between two distinct device types in the same game.  

### Math
These functions are defined in the base language and they take one or more arguments.  
If the math function is used after a `=` sign in a variable assignation statement, it will return the result to that variable and will NOT modify the value of any of its arguments.  
For trailing math functions (used in a variable assignment without the `=` word), it will use the leading variable as its first argument and modify that variable with the return value.  
- `floor` number
- `ceil` number
- `round` number (takes an optional second argument to specify the number of decimals, otherwise rounding to integer)
- `sin` number
- `cos` number
- `tan` number
- `asin` number
- `acos` number
- `atan` number
- `abs` number
- `fract` number
- `log` number
- `log10` number
- `sqrt` number
- `sign` number
- `pow` number number
- `clamp` number number number
- `step` number number number
- `lerp` number number number
- `slerp` number number number
- `mod` number number (the modulo operator)
Math functions below may take two numeric arguments or more. It may also take one array except if it's a trailing function call.
- `min` number number ...
- `max` number number ...
- `avg` number number ...
- `med` number number ...
- `add` number number ...
- `sub` number number ...
- `mul` number number ...
- `div` number number ...

An implementation may add additional math functions for more advanced fonctionality, or even exclude some of them from this list. It is however discouraged to modify the standard behaviour of a base math function.  

# User-Defined Functions

Functions define a group of operations that we may call one or more times during the execution of the program. 

The operations to run in a function appear within its body. 

Functions may have arguments that can be used within the body so that the operations may have a variation depending on the value of some variables. 

Function arguments are defined after the function name and they can be of type `number`, `text`, or a special `data` type. 

### Declaration

Functions may be declared multiple times with different argument types or different number of arguments.  
This is similar to function overloading in some programming languages.  

Here are some function declaration examples.  

This function takes in a single number argument:
```function @func1 $var1:number```

This function takes two number arguments:
```function @func1 $var1:number $var2:number```

This function takes a number argument and a text argument:
```function @func1 $var1:number $var2:text```

This function takes a special data argument:
```function @func1 $var1:data```

### Body
The body of a function (the operations to be executed when calling it) must be on the following lines after the declaration, indented with one tab.  
Empty lines within a body are allowed and ignored by the compiler.  
Functions bodies may have a `return` statement optionally followed by an expression that would be used to assigned a leading variable.

```
function @func1 $var1:number $var2:number
    return $var1 + $var2
```

### Call

Calling a function will run the operation in its body. 
To call a function, simply write the name of the function starting with `@`, then its arguments, like so:  
```@funcName 4 6```
Here we have passed two number arguments, thus this call execute the body of the second declaration above.  
It is of course also possible to use variables instead of literal numbers as the function arguments.  

### Return value

Functions may return a value using the `return` keyword.  
This returned value may be assigned to a variable like so : 
```$value = @funcName 4 6```

# Compiler Specifications
This section is intented for game developers who want to use this in their game

## Editor
It is encouraged that the code editor runs the following parse on the current line on each keystroke: 
- Replace leading sequences of 2-5 spaces to one tab
- Add a space before and after a non-leading `;` or `//` that were not already within comments or within string literals
- Add a space before and after operators that are not within comments or string literals
- Add a space before the starting quote and after the ending quote of a literal string expression ` "..." `
- Remove spaces before and after `:` and `.` characters that are not within comments or within string literals
- Remove spaces before `#` characters that are not within comments or within string literals
- Trim trailing white spaces
- Trim all duplicate spaces that are not within comments or string literals
- Autocomplete words upon pressing the space bar if that word is not an existing symbol
    - Write the minimum remaining characters (common denominator) of the symbols that start with the written characters
        - If this is the leading word: Only autocomplete with global scope statement words
        - If preceded with tabs: Only autocomplete with function body statement words
        - When the first character in the current word is a `$`, autocomplete with a user variable
        - When the first character in the current word is a `@`, autocomplete with a user function
        - Otherwise, Autocomplete with one of:
            - User-defined variables
            - User-defined functions
            - Current function/foreach/repeat arguments
            - Built-in functions
            - Operators

## Run
Upon powering up the virtual computer:
- Execute the body of the Init() function
One clock cycle, executed 'frequency' times per second:
- Execute all timer functions sequentially if their time is due
- Execute all input functions that have received some data since the last cycle
- Sleep for Elapsed-1/Frequency Seconds

