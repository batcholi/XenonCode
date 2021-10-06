# XenonCode - Documentation

**XenonCode** is a lightweight programming language designed as high level scripting within games for virtual computing devices (ie: a programmable computer within a game).

# Capabilities

* Number and Text variables
* Fixed size Arrays defined in the global scope
* All standard arithmetic operations on numeric values
* String concatenation operators
* User-defined functions with overloadable arguments
* Device-defined functions
* Synchronized interval functions/timers (timer functions)
* Built-in IO operations for inter-device data transfer
* Built-in standard math functions
* `if`/`elseif`/`else` conditionals
* `foreach` loops for arrays
* `repeat n` loops (similar to a `for (i=0;i<n;i++)` in other languages)

# Syntax

- XenonCode is designed with a very basic syntax in mind.
- Each statement is to be very short and easy to read.
- Very little special characters needed
- No semicolon at the end of each statement
- Indentations define the scope (tabs)
- Forced to have spaces between expressions and operators
- A single statement per line
- 100% Case-insensitive

### Types

XenonCode is a typed procedural language with overload resolution, but only includes two generic types, as well as arrays or either, and special opaque data types.  

Generic data types that the user can declare: 
* Number
* Text

A `number` variable is always a 64-bit floating point internally, but may also act as a boolean when its value is either 1 or 0 (true or false).  

`text` variables contain pure arbitrary text, altough they may have a limited size depending on the implementation.  

Special `data` types are for use by the implementation and are opaque to the user, meaning they cannot directly be decomposed by the program, their structure is not specifically defined, and cannot be declared by a user. They can only be passed around from one function call to another. They may also support member functions. 

### Comments

Comments are lines that start with either `;` or `//`  
A code statement may also have a trailing comment

# Limitations

- No infinite loops, no while loops,... only fixed loops that are fully unrolled upon code compilation
- No pointer nor reference types
- Arrays are fixed size and must be declared in the global scope
- No recursive call of the same function

### Per-Chip limitations
- ROM (max program size in compiled word count)
- RAM (max number of arrays, multiplied by their size)
- Timers (max number of timer functions)
- Frequency (max frequency for timer functions and input read)
- Number of ports (max input/output number)

### Operation on data
- All functions, including timers, are executed atomically, preventing any data-race situation.
- Function arguments are always passed by reference (exception below)
- Arguments going from an `output` to an `input` function transfers data by copy
- Variable assignations are always by copy
- Math functions do not modify their given arguments, they will instead return a new value

# Valid usage

XenonCode is also designed to be very easy to parse. 

## Each line must begin with one of the following first word (with examples) :

### Global scope
- `init` the body of this function will execute once, when the device is powered on
- `timer` `10 Hz` the body of these functions will execute repeatedly at a specific frequency in Hz
- `input` `1 var1:number var2:text` define an input, its arguments and its body
- `var` `numberVar1:number 4` define a global number variable with an (optional) initial value
- `var` `textVar1:text "hello"` define a global text variable with an (optional) initial value
- `const` `numberConstant1:number 4` define a global number constant with an initial value
- `array` `10 numberArray1:number 0` define an array of 10 numbers initialized to 0
- `function` `testFunc var1:text` define a function and its body

### Function body
- `set` `textVar1 = "hello"`
- `set` `numberVar1 = 4`
- `set` `numberVar1 to 4`
- `set` `numberVar1 multiplied 2`
- `set` `numberVar1 / 3`
- `set` `numberArray1:4 = 0`
- `var` `numberVar1:number = 4` define a local number variable
- `output` `2 "test"`
- `exec` `func1 arg1` calls a user-defined function
- `device` `"nativeCodeFunc1" arg1 arg2` calls a device-defined native function
- `foreach` `item in numberArray`
    - `break` stops the loop here as if it completed all iterations
    - `next` stops this iteration here and run the next iteration immediately
- `repeat` `10 n`
    - `break`
    - `next`
- `if` `var1 is 4`
- `elseif` `var1 is 5`
- `else`
- `clear` `var1` clears the content of a variable (optionally takes an argument as the clear value)
- `increment` `numberVar1` increments the value of a variable by 1 (optionally takes an argument to increment by another number instead)
- `decrement` `numberVar1` decrements the value of a variable by 1 (optionally takes an argument to decrement by another number instead)
- `return` `numberVar1` stop the execution of this function here and return a value to the caller

## Reserved keywords
The following keywords must NOT be used to define a variable or a function
- `exec`
- `device`
- `math`
- Any of the operators below

# Operators
- `=` or `to` variable assignation (must follow a `set variableName`)
- `+` or `plus` number addition
- `-` or `minus` number subtraction
- `*` or `multiplied` number multiplication
- `/` or `divided` number division
- `%` or `modulo` number modulo
- `==` or `is` equal to
- `!=` or `not` not equal to
- `>` or `moreThan` greater than
- `<` or `lessThan` lesser than
- `<=` or `atMost` lesser or equal to
- `>=` or `atLeast` greater or equal to
- `&&` or `and` conditional AND
- `||` or `or` conditional AND
- `(` begin an enclosed expression
- `)` end an enclosed expression

## Casting (parse variables as another type)

To parse an existing variable as another type, simply add a colon and the type, like so: 
```set someTextValue = someNumberValue:text```
or
```set someTextValue = ( numberValue1 + numberValue2 ):text```
This only works with simple variable types `number` and `text`, and enclosed expressions

## String concatenation

To concatenate texts, simply compound all text values/variables in the same expression (don't forget to cast to text).  

Example concatenating a text as a single argument of an output function call:
```output 1 ( "Hello, my name is " someTextVar " and my age is " someNumberVar:text )`

Example concatenating a text while assigning to a variable
```set someTextVar to "Hello, " myName "!"```

# Built-in functions

These functions should be used directly within expressions, prefixed by the `math` keyword. They take one or more arguments and return one value.

### Math
- `round` number
- `floor` number
- `ceil` number
- `round` number
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
- `min` number number
- `max` number number
- `avg` number number

`min`, `max` and `avg` may take two numeric arguments OR an array of numbers.  

`round` takes an optional second argument to specify the number of decimals (otherwise round to integer)  

# User-Defined Functions

Functions define a group of operations that we may call one or more times during the execution of the program. 

The operations to run in a function appear within its body. 

Functions may have arguments that can be used within the body so that the operations may have a variation depending on the value of some variables. 

Function arguments are defined after the function name and they can be of type `number`, `text`, an array of either, or a special `data` type. 

### Declaration

Functions may be declared multiple times with different argument types or different number of arguments. 
This is similar to function overloading in some programming languages. 

Here are some function declaration examples.

This function takes in a single number argument:
```function func1 var1:number```

This function takes two number arguments:
```function func1 var1:number var2:number```

This function takes a number argument and a text argument:
```function func1 var1:number var2:text```

This function takes a special data argument:
```function func1 var1:data```

This function takes an array of numbers argument:
```function func1 var1:array:number```

### Body
The body of a function (the operations to be executed when calling it) must be on the following lines after the declaration, indented with one tab.
Empty lines within a body are allowed and ignored by the compiler.

```
function func1 var1:number var2:number
    set globalVar1 = var1 + var2
    return globalVar1
```

### Call

Calling a function will run the operation in its body. 
To call a function, simply write `exec` followed by the name of the function, then its arguments, like so : 
```exec funcName 4 6```
Here we have passed two number arguments, thus this call execute the body of the second declaration above.  
It is of course also possible to use variables instead of literal numbers as the function arguments, or even entire enclosed expressions

### Return value

Functions may return a value using the `return` keyword.  
This returned value may be assigned to a variable like so : 
```set value = exec funcName 4 6```
Or passed as one of another function call arguments by surrounding it with parenthesis like so:
```exec someOtherFunction arg1 ( exec funcName 4 6 ) arg3```

# Compiler Specifications
This section is intented for game developers who want to use this in their game

## Editor
It is encouraged that the code editor runs the following parse on the current line on each keystroke: 
- Replace leading sequences of 2-5 spaces to one tab
- Add a space before and after a non-leading `;` or `//` that were not already within comments or within literal strings
- Add a space before and after operators that are not within comments or literal strings
- Add a space before the starting quote and after the ending quote of a literal string expression ` "..." `
- Add a space before and after the enclosed expression parenthesis `(` `)`
- Remove spaces before and after a `:` character that is not within comments or within literal strings
- Trim trailing white spaces
- Trim all duplicate spaces that are not within comments or string literals
- Autocomplete words upon pressing the space bar if that word is not an existing symbol
    - Write the minimum remaining characters (common denominator) of the symbols that start with written characters
        - If this is the leading word: Only autocomplete with global scope statement words
        - If preceded with tabs: Only autocomplete with function body statement words
        - If the previous word was `set`, `clear`, `increment` or `decrement`: Only autocomplete user-defined variables and current function/foreach/repeat arguments
        - If the previous word was `exec`: Only autocomplete user-defined functions
        - If the previous word was `device`: Only autocomplete device functions
        - If the previous word was `math`: Only autocomplete math functions
        - Otherwise, Autocomplete with one of:
            - User-defined variables
            - Current function/foreach/repeat arguments
            - Operators
            - `exec`, `device`, `math`
    - If the written word was a leading existing user-defined function preceded by tabs: Add `exec` in front of it
    - If the written word was a leading existing device function preceded by tabs: Add `device` in front of it
    - If the written word was a leading existing user-defined variable preceded by tabs: Add `set` in front of it
    - 

## Compiling
It is encouraged to compile the scripts to bytecode so that it can be parsed easily at runtime.  
* Allocate a global array for string literals
* Allocate an stacks wich will contain 64-bit floating point values, strings and byte arrays
    * Any use of a variable will reference the one with the same name on a parent stack or create a new variable on this stack only.
* A C++ example of a stack allocation would be: 
    ```
    vector<double> numberValues;
    vector<string> stringValues;
    vector<vector<double>> numberArrays;
    vector<vector<string>> stringArrays;
    unordered_map<string/*functionName*/, vector<int>> functions; // contains the functions byteCode
    map<uint8_t/*stackIndex*/, unordered_map<string/*variableName*/, tuple<type, int/*index*/>>> references;
    vector<double> timerFrequencies;
    ```
* Keep track of the line number (to print useful error messages to the user)
* Keep track of current stack index (number of tabs), the current function call stack, whether the init function has been defined, and the number of timers.
    * Start at stack index 0 with empty function stack (Global Scope)
    ```
    bool initDefined = false;
    int nbTimers = 0;
    int stackIndex = 0;
    stack<string> functionStack;
    stack<vector<string>> argStack;
    ```

The following must be executed for each line:

* Comments or empty lines: Ignore the line if it matches the regex pattern `^\s*(;|//|$)`
* Replace all occurences of string literals with `$123` (123 is the index of that string constant reference)
* Remove trailing spaces and comments using the regex pattern `\s*((;|//).*)?$`
* Convert the entire line to lowercase
* BEGIN:
* If stackIndex is 0:
    * Confirm that there are no leading tabs on the current line, otherwise throw a syntax error.
    * Line must start with one of the following, with no leading tab/space, otherwise it's a syntax error. 
        * `init`
            * Confirm EOL, or throw syntax error
            * If initDefined is true, throw an error "init function is already defined in this device"
            * Increment stackIndex
            * push "init" to functionStack
            * set initDefined to true
        * `timer`
            * If nbTimers >= MaxNumberOfTimersForThisDevice, throw an error "number of timers defined exceeds the number of timers on this device"
            * Read "%f hz" and add it to timerFrequencies
            * Confirm EOL, or throw syntax error
            * Increment nbTimers
            * Increment stackIndex
            * push "timer 1" to functionStack (1 would be the timer index based on the current value of nbTimers)
        * `input`
            * Read one int value as a 1-based input index
            * Read argument expressions
            * Increment stackIndex
            * push "input 1 n t" to functionStack (1 would be the input index read above, 'n' and 't' are exemples of function argument types for number and text)
            * push argument names to argStack
        * `function`
            * Read one string value as the name of this function
            * Read argument expressions
            * Increment stackIndex
            * push "funcName n t" to functionStack (@funcName would be the function name read above, 'n' and 't' are exemples of function argument types for number and text)
            * push argument names to argStack
        * `var` or `const`
            * Read "varName:varType", or syntax error
            * If EOL: set the initial value of the variable to 0 or "" (or syntax error if it's a `const`)
                * otherwise: Read a literal value, then set that as the initial value
            * append the variable to either `numberValues` or `stringValues` if the type is `number` or `text` respectively
        * `array`
            * Read one integral value as the number of elements in this array
            * Read "varName:varType", or syntax error
            * If EOL: set the initial value of all the elements of this array to 0 or ""
                * otherwise: Read a literal value, then set that as the initial value for all the elements of this array
            * append the variable to either `numberArrays` or `stringArrays` if the type is `number` or `text` respectively
* If stackIndex > 0:
    * using stackPop = stackIndex - numberOfLeadingTabsOnCurrentLine
    * Confirm stackPop >= 0, or throw syntax error
    * pop stackPop times from functionStack
    * decrement stackIndex by stackPop
    * if stackIndex == 0: goto BEGIN
    * Parse line into the function body that's on top of functionStack
        * Line must start with one of the following, otherwise it's a syntax error. 
            * `clear`
            * `increment`
            * `decrement`
            * `set`
            * `var`
            * `output`
            * `exec`
            * `device`
            * `foreach`
                * add a call to exec, with "currentLineNumber" as the function name and the loop index as the argument
                * increment stackIndex and push "currentLineNumber n" to functionStack
            * `repeat`
                * add a call to exec, with "currentLineNumber" as the function name and the loop index as the argument
                * increment stackIndex and push "currentLineNumber n" to functionStack
            * `if`
                * add a call to exec, with "currentLineNumber" as the function name
                * increment stackIndex and push "currentLineNumber" to functionStack
            * `elseif`
                * add a call to exec, with "currentLineNumber" as the function name
                * increment stackIndex and push "currentLineNumber" to functionStack
            * `else`
            * `break`
            * `next`
            * `return`

## Run
* Execute the body of the Init() function (only once per power cycle)
* Execute all timer functions sequentially if their time is due
* Execute all input functions that have received some data
* Sleep for Elapsed-1/Frequency Seconds

