# XenonCode - Documentation

**XenonCode** is a lightweight programming language designed as high level scripting within games for virtual computing devices (ie: a programmable computer within a game).

# Capabilities

XenonCode is a very lightweight language designed to be easy to learn. 

* Numeric and Text variables with support for Arrays of fixed size
* Any standard arithmetic operation on number values
* Functions with arguments and overload resolution
* Basic Synchronized Multicoreing (core functions)
* Convenient number suffixes for unit conversions
* Built-in IO operations for inter-device data transfer
* Built-in standard math library
* `if`/`elseif`/`else` conditionals
* `foreach` loops for arrays
* `repeat n` loops (similar to `for (i=0;i<n;i++)` in other languages)
* All function arguments are always by reference, assignations are by copy

# Syntax

- XenonCode is designed with a very basic syntax in mind.
- Each statement is to be very short and easy to read.
- Very little special characters needed
- No semicolon at the end of each statement.
- Indentations define the scope (tabs)
- Forced to have spaces between each expression
- A single statement per line
- 100% Case-insensitive

### Types

XenonCode is a typed procedural language with overload resolution, but only supports two types, as well as arrays and functions. 

* Number
* Text

A number value is always a 64-bit floating point internally, but may also act as a boolean when its value is either 1 or 0 (true or false).

Texts may have a limited size depending on the implementation.

### Comments

Comments are lines that start with either `;` or `//`  
A code statement may also have a trailing comment

# Limitations

- No infinite loops, no while loops,... only fixed loops
- No pointer nor reference types
- Arrays are fixed size and must be declared in the global scope
- No recursive call of the same function

### Per-Chip limitations
- ROM (max program size in characters count)
- RAM (max number of arrays, multiplied by their size)
- Cores (max number of core functions)
- Frequency (max frequency for core functions)
- Number of ports (max input/output number)

# Valid usage

XenonCode is also designed to be very easy to parse. 

## Each line must begin with one of the following examples :

### Global scope
- `init` the body of this function will execute once, when the device is powered on
- `core` `frequency 10 Hz` the body of these functions will execute repeatedly at a specific frequency in Hz
- `input` `1 var1:number var2:text` define an input and its body
- `var` `numberVarName:number = 4` define a global number variable
- `var` `textVarName:text = "hello"` define a global text variable
- `array` `10 numberArrayName:number = 0` define an array (uses RAM)
- `function` `testFunc var1:text` define a function and its body

Each `core` body is executed atomically, preventing any data-race.

### Function body
- `set` `textVarName = "hello"`
- `set` `numberVarName = 4`
- `set` `numberArrayName:4 = 0`
- `var` `numberVarName:number = 4` define a local number variable
- `output` `2 "test"`
- `exec` `funcName arg1`
- `exec` `"nativeCodeFuncName" arg1 arg2`
- `foreach` `item in numberArray`
    - `break`
    - `next`
- `repeat` `10 n`
    - `break`
    - `next`
- `if` `var1 is 4`
- `elseif` `var1 is 5`
- `else`
- `clear`
- `increment`
- `decrement`

# Built-in functions

### System
- `size` arr
- `clear` var
- `increment` var
- `decrement` var

### Math
- `round` number
- `min` number...
- `max` number...
- `avg` number...
- `floor` number
- `ceiling` number
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
- `pow` number number
- `sqrt` number
- `sign` number
- `clamp` number number number
- `step` number number number
- `lerp` number number number
- `slerp` number number number

# Operators
- `+` or `plus` number addition
- `-` or `minus` number subtraction
- `*` or `times` number multiplication
- `/` or `dividedBy` number division
- `%` or `modulo` number modulo
- `==` or `is` equal to
- `!=` or `isNot` not equal to
- `>` or `isMoreThan` greater than
- `<` or `isLessThan` lesser than
- `<=` or `isAtMost` lesser or equal to
- `>=` or `isAtLeast` greater or equal to
- `&&` or `and` conditional AND
- `||` or `or` conditional AND

# Functions

Functions define a group of operations that we may call one or more times during the execution of the program. 

The operations to run in a function appear within its body. 

Functions may have arguments that can be used within the body so that the operations may have a variation depending on the value of some variables. 

Function arguments are defined within parenthesis, they can be of type number, text or an array of either. 

### Declaration

Functions may be declared multiple times with different argument types or different number of arguments. 
This is similar to function overloading in some programming languages. 

All function declarations must have a body, except the `input` functions which have a default body that may or may not do anything, if none is given. 

Here are some function declaration examples.

This function takes in a single number argument :
```function funcName var1:number```

This function takes two number arguments :
```function funcName var1:number var2:number```

This function takes a number argument and a text argument :
```function funcName var1:number var2:text```

### Body
The body of a function (the operations to be executed when calling it) must be on the following lines after the declaration, indented with exactly one tab.
Empty lines within a body are allowed and ignored by the compiler.

```
function funcName var1:number var2:number
    set globalVar1 = var1 + var2
    return globalVar1
```

### Call

Calling a function will run the operation in its body. 
To call a function, simply write `exec` followed by the name of the function, then its arguments, like so : 
```exec funcName 4 6```
Here we have passed two number arguments, thus this call execute the body of the second declaration above. 

### Return value

Functions may return a value using the `return` keyword.  
This returned value may be assigned to a variable like so : 
```set value = exec funcName 4 6```
Or passed as another function's argument by surrounding it with parenthesis like so:
```exec someOtherFunction ( exec funcName 4 6 ) someOtherFunctionSecondArg```

# Casting (parse variables as another type)

To parse an existing variable as another type, simply add a colon and the type, like so: 
```
set someTextValue = someNumberValue:text
```

# Number value suffixes

Number values may have a suffix, this is useful to convert from one unit to another or use a specific unit as argument in a function call. 

Suffixes are simply a shortcut that multiplies the given value so that the result is in the default unit (SI base unit). 

Example : converting 5 kilometers to feet
```set distanceInFeet = 5km / 1ft```
or
```set distanceInFeet = value * 1km / 1ft```

### Time : 
- `ms` = milliseconds
- `s` = seconds (default)
	
### Distance / Length : 
- `mm` = millimeters
- `cm` = centimeters
- `in` = inches
- `ft` = feet
- `m` = meters (default)
- `km` = kilometers
- `au` = astronomical unit
- `ly` = light-years

### Mass : 
- `mg` = milligrams
- `kg` = kilograms (default)
- `lbs` = pounds

### Density :
- `kgm` = kilogram per meter cube (default)

### Pressure :
- `pa` = pascal (default)
- `kpa` = kilopascal
- `psi` = Pound per square inch
- `bar` = standard earth atmospheric pressure

### Speed :
- `mps` = meters per second (default)
- `kps` = kilometers per second
- `kph` = kilometers per hour
- `kts` = knots
- `mph` = miles per hour

### Temperature :
- `k` = kelvin (default)
- `c` = celcius
- `f` = Fahrenheit

### Current
- `ma` = milli-ampere
- `a` = ampere (default)

### Voltage
- `mv` = millivolt
- `v` = volt (default)
- `kv` = kilovolt

### Energy
- `mj` = millijoule
- `j` = joule (default)
- `kj` = kilojoule
- `wh` = watt-hour
- `kwh` = kilowatt-hour

### Power
- `mw` = milliwatt
- `w` = watt (default)
- `kw` = kilowatt

# Compiler Specifications
This section is intented for game developers who want to use this in their game

## Editor
It is encouraged that the code editor runs the following parse on the current line on each keystroke: 
* Replace groups of 2-4 spaces to tabs
* Trim all duplicate spaces that are not within comments
* Trim trailing spaces and tabs

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
    vector<double> coreFrequencies;
    ```
* Keep track of the line number (to print useful error messages to the user)
* Keep track of current stack index (number of tabs), the current function call stack, whether the init function has been defined, and the number of cores.
    * Start at stack index 0 with empty function stack (Global Scope)
    ```
    bool initDefined = false;
    int nbCores = 0;
    int stackIndex = 0;
    stack<string> functionStack;
    stack<vector<string>> argStack;
    ```

The following must be executed for each line:

* Comments or empty lines: Ignore the line if it matches the regex pattern `^\s*(;|//|$)`
* Replace all occurences of string literals with `$123` (123 is the index of that string literal reference)
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
        * `core`
            * If nbCores >= MaxNumberOfCoresForThisDevice, throw an error "number of cores defined exceeds the number of cores on this device"
            * Read "frequency %f hz" and add it to coreFrequencies
            * Confirm EOL, or throw syntax error
            * Increment nbCores
            * Increment stackIndex
            * push "core 1" to functionStack (1 would be the core index based on the current value of nbCores)
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
        * `var`
            * Read "varName:varType", or syntax error
            * If EOL: set the initial value of the variable to 0 or ""
                * otherwise: Read a `=` and a literal value, then set that as the initial value
            * append the variable to either `numberValues` or `stringValues` if the type is `number` or `text` respectively
        * `array`
            * Read one integral value as the number of elements in this array
            * Read "varName:varType", or syntax error
            * If EOL: set the initial value of all the elements of this array to 0 or ""
                * otherwise: Read a `=` and a literal value, then set that as the initial value for all the elements of this array
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
* Execute all core functions sequentially if their time is due
* Execute all input functions that have received some data
* Sleep for Elapsed-1/Frequency Seconds

