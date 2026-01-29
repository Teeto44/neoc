# NeoC Language Specification
**Version 0.0.1-DEV**

A modern, statically-typed systems programming language designed for simplicity, portability, and performance.

---

## Table of Contents
1. [Introduction](#introduction)
2. [Language Philosophy](#language-philosophy)
3. [Source Files](#source-files)
4. [Lexical Elements](#lexical-elements)
5. [Comments](#comments)
6. [Identifiers](#identifiers)
7. [Keywords](#keywords)
8. [Types](#types)
9. [Literals](#literals)
10. [Operators](#operators)
11. [Statements](#statements)
12. [Scopes](#scopes)
13. [Variables](#variables)
14. [Functions](#functions)
15. [Control Flow](#control-flow)
16. [Example Programs](#example-programs)

---

## Introduction

NeoC is a systems programming language created to serve as a modern replacement for C as the lowest-common-denominator language for library APIs and cross-language interoperability. While C has served this role admirably for decades, NeoC aims to provide the same portability and simplicity with modern language features and improved ergonomics.

---

## Language Philosophy

NeoC follows these core principles:

1. **Explicit over implicit**: The code should clearly express the programmer's intent
2. **Opinionated**: NeoC has a strong opinion about how code should be written, formatted, and what are good and bad features.
2. **Simplicity**: Clean, readable syntax. Good practices are enforced by the compiler.
2. **Safe by default**: By default, Variables are immutable, Functions and types are private, and the compiler will never implicitly perform lossy casting. Defaults should be safe and reliable.
3. **Zero-cost abstractions**: Modern features shouldn't sacrifice performance
4. **Predictable behavior**: No hidden costs or surprising behavior
5. **Minimal runtime**: No garbage collector or heavy runtime requirements

---

## Source Files

### File Extensions
NeoC source files use one of two file extensions:
- `.nc` (preferred)
- `.neoc` (alternative)

These extensions make NeoC files immediately recognizable and distinguish them from other programming languages.

---

## Lexical Elements

### Whitespace
Whitespace characters include:
- Space (` `)
- Tab (`\t`)
- Carriage return (`\r`)
- Newline (`\n`)

Whitespace is used to separate tokens but is otherwise ignored by the compiler. Any amount of whitespace is equivalent for parsing purposes.

---

## Comments

Comments are portions of source code that are ignored by the compiler and serve as documentation for reading the code.

### Single-Line Comments
Single-line comments begin with `//` and continue until the end of the line.

```neoc
// This is a single-line comment
i32 x = 42; // This comment follows a statement
```

### Multi-Line Comments
Multi-line comments begin with `/*` and end with `*/`. They can span multiple lines.

```neoc
/*
This is a multi-line comment.
It can span many lines.
*/

/* This is also valid on a single line */
```

**Note**: Multi-line comments do **not** nest. The first `*/` encountered will close the comment.

### Documentation Comments
Documentation comments begin with `///` and must appear on the line immediately before the element they document (functions, types, etc.). These comments are used by documentation generators and IDEs to provide context-sensitive help.

```neoc
/// Calculates the sum of two integers.
/// Returns the sum as an i32.
fn add(i32 a, i32 b) i32 {
    return a + b;
}
```

---

## Identifiers

Identifiers are names used for variables, functions, types, and other program elements.

### Naming Rules
An identifier must follow these rules:

1. **First character**: Must be a letter (`a-z`, `A-Z`) or underscore (`_`)
2. **Subsequent characters**: May be letters, digits (`0-9`), or underscores
3. **Case sensitivity**: Identifiers are case-sensitive (`myVar` and `MyVar` are different)
4. **Length**: No maximum length (implementation-dependent, but guaranteed at least 255 characters)
5. **Reserved words**: Cannot be a keyword (see [Keywords](#keywords))

### Valid Identifiers
```neoc
x
myVariable
_privateVar
test123
my_long_variable_name
__internal__
variable_2
```

### Invalid Identifiers
```neoc
123invalid  // Cannot start with a digit
my-variable // Hyphens are not allowed
my variable // Spaces are not allowed
fn          // Cannot use keywords
my.var      // Dots are not allowed
```

**Note**: Although all of these examples will be accepted by the compiler, that does not imply they are truly correct by NeoC standards, or follow its conventions.

---

## Keywords

The following words are reserved and cannot be used as identifiers:

### Control Flow Keywords
- `if` - Conditional execution
- `else` - Alternative branch in conditionals
- `return` - Exit function and optionally return a value

### Declaration Keywords
- `fn` - Function declaration
- `mut` - Mutable variable modifier

### Type Keywords
- `i8`, `i16`, `i32`, `i64`, `i128` - Signed integer types
- `u8`, `u16`, `u32`, `u64`, `u128` - Unsigned integer types
- `f32`, `f64` - Floating-point types
- `bool` - Boolean type
- `char` - Character type

---

## Types

NeoC is a statically-typed language, meaning every variable and expression has a type known at compile time.
The compiler will never infer a type for you, so you must explicitly specify the type of every variable you declare.
This doesn't necessarily apply to expressions, as their type can sometimes be inferred.

### Integer Types

#### Signed Integers
Signed integers can represent both positive and negative values.

| Type | Size | Range |
|------|------|-------|
| `i8` | 8 bits | -128 to 127 |
| `i16` | 16 bits | -32,768 to 32,767 |
| `i32` | 32 bits | -2,147,483,648 to 2,147,483,647 |
| `i64` | 64 bits | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| `i128` | 128 bits | ±1.7×10^38 (approx) |

#### Unsigned Integers
Unsigned integers can only represent non-negative values.

| Type | Size | Range |
|------|------|-------|
| `u8` | 8 bits | 0 to 255 |
| `u16` | 16 bits | 0 to 65,535 |
| `u32` | 32 bits | 0 to 4,294,967,295 |
| `u64` | 64 bits | 0 to 18,446,744,073,709,551,615 |
| `u128` | 128 bits | 0 to 3.4×10^38 (approx) |

### Floating-Point Types

| Type | Size | Precision | Range |
|------|------|-----------|-------|
| `f32` | 32 bits | ~7 decimal digits | ±3.4×10^38 (approx) |
| `f64` | 64 bits | ~15 decimal digits | ±1.7×10^308 (approx) |

Floating-point types follow the IEEE 754 standard.

### Boolean Type

The `bool` type has two possible values:
- `true`
- `false`

Booleans are returned by comparison and logical operations and used in conditional statements.

### Character Type

The `char` type represents a single ASCII character (8 bits). Character literals are enclosed in single quotes.

---

## Literals

Literals are constant values written directly in the source code.

### Integer Literals

Integer literals are sequences of digits representing whole numbers.

```neoc
0       // Zero
42      // Positive integer
999999  // Large integer
```

**Rules**:
- Must contain at least one digit
- Cannot start with `0` unless the value is exactly `0` (no octal notation)
- No separators allowed (e.g., `1_000` is invalid)
- Negative literals use the unary minus operator (e.g., `-42`)

**Type inference**: Integer literals default to `i32` unless the context requires otherwise.

### Floating-Point Literals

Floating-point literals represent real numbers and must contain a decimal point.

```neoc
0.0
3.14
123.456
0.5
```

**Rules**:
- Must contain a decimal point (`.`)
- Must have at least one digit before and after the decimal point
- Scientific notation is not supported

**Type inference**: Floating-point literals default to `f64` unless the context requires otherwise.

### Boolean Literals

Boolean literals are the keywords `true` and `false`.

```neoc
bool isActive = true;
bool hasError = false;
```

### Character Literals

Character literals are single characters enclosed in single quotes (`'`).

```neoc
char letter = 'a';
char digit = '5';
char symbol = '$';
```

**Note**: Character literals containing non-ASCII characters (Unicode code points > 127) are lossily converted to ASCII
by dropping the leading bits.

#### Escape Sequences

Special characters can be represented using escape sequences:

| Escape | Meaning | ASCII Value |
|--------|---------|-------------|
| `\n` | Newline (Line Feed) | 10 |
| `\t` | Horizontal Tab | 9 |
| `\r` | Carriage Return | 13 |
| `\\` | Backslash | 92 |
| `\'` | Single Quote | 39 |
| `\0` | Null Character | 0 |

```neoc
char newline = '\n';
char tab = '\t';
char quote = '\'';
char backslash = '\\';
```

**Invalid Character Literals**:
```neoc
''      // Empty - invalid
'ab'    // Multiple characters - invalid
'       // Unclosed - invalid
'\x'    // Invalid escape sequence
```

---

## Operators

Operators are symbols that perform operations on one or more operands.

### Arithmetic Operators

| Operator | Operation | Example | Description |
|----------|-----------|---------|-------------|
| `+` | Addition | `a + b` | Adds two values |
| `-` | Subtraction | `a - b` | Subtracts second from first |
| `*` | Multiplication | `a * b` | Multiplies two values |
| `/` | Division | `a / b` | Divides first by second |
| `%` | Modulo | `a % b` | Remainder of division |

```neoc
i32 sum = 10 + 5;        // 15
i32 difference = 10 - 5; // 5
i32 product = 10 * 5;    // 50
i32 quotient = 10 / 5;   // 2
i32 remainder = 10 % 3;  // 1
```

**Integer Overflow**: Arithmetic operations wrap on overflow using two's complement arithmetic (modulo 2^n where n is the bit width).

**Integer Division**: Division truncates toward zero. Division or modulo by zero causes a runtime panic.

**Modulo**: Takes the sign of the left operand (dividend).

**Examples**:
```neoc
i8 x = 127;
i8 y = x + 1;  // y = -128 (wraps)

u8 a = 255;
u8 b = a + 1;  // b = 0 (wraps)

i32 c = 10 / 0;  // Runtime panic
```

### Comparison Operators

| Operator | Operation | Example | Result Type |
|----------|-----------|---------|-------------|
| `==` | Equal to | `a == b` | `bool` |
| `!=` | Not equal to | `a != b` | `bool` |
| `<` | Less than | `a < b` | `bool` |
| `>` | Greater than | `a > b` | `bool` |
| `<=` | Less than or equal | `a <= b` | `bool` |
| `>=` | Greater than or equal | `a >= b` | `bool` |

```neoc
bool equal = (5 == 5);      // true
bool notEqual = (5 != 3);   // true
bool less = (3 < 5);        // true
bool greater = (5 > 10);    // false
bool lessEq = (5 <= 5);     // true
bool greaterEq = (10 >= 5); // true
```

**Floating-Point Comparisons**: Comparisons follow IEEE 754 semantics. Notably, `NaN` is not equal to itself:
```neoc
f64 nan = 0.0 / 0.0;  // NaN
bool result = nan == nan;  // false
```

### Logical Operators

| Operator | Operation | Example | Description |
|----------|-----------|---------|-------------|
| `&&` | Logical AND | `a && b` | True if both are true |
| `\|\|` | Logical OR | `a \|\| b` | True if either is true |
| `!` | Logical NOT | `!a` | Inverts boolean value |

```neoc
bool and = true && false;     // false
bool or = true || false;      // true
bool not = !true;             // false
bool complex = (true && false) || (!false && true); // true
```

**Short-circuit evaluation**: `&&` and `||` use short-circuit evaluation. The right operand is only evaluated if necessary.

### Assignment Operators

| Operator | Operation | Example | Equivalent To |
|----------|-----------|---------|---------------|
| `=` | Assignment | `a = b` | Assign b to a |
| `+=` | Add and assign | `a += b` | `a = a + b` |
| `-=` | Subtract and assign | `a -= b` | `a = a - b` |
| `*=` | Multiply and assign | `a *= b` | `a = a * b` |
| `/=` | Divide and assign | `a /= b` | `a = a / b` |
| `%=` | Modulo and assign | `a %= b` | `a = a % b` |

```neoc
mut i32 x = 10;
x += 5;    // x is now 15
x -= 3;    // x is now 12
x *= 2;    // x is now 24
x /= 4;    // x is now 6
x %= 4;    // x is now 2
```

**Note**: Assignment operators require the variable to be declared as `mut` (mutable).

### Unary Operators

| Operator | Operation | Example | Description |
|----------|-----------|---------|-------------|
| `++` | Increment | `++a` or `a++` | Increase by 1 |
| `--` | Decrement | `--a` or `a--` | Decrease by 1 |
| `-` | Negation | `-a` | Arithmetic negation |
| `!` | Logical NOT | `!a` | Boolean negation |

```neoc
mut i32 x = 5;
x++;       // Postfix: x becomes 6
++x;       // Prefix: x becomes 7
x--;       // Postfix: x becomes 6
--x;       // Prefix: x becomes 5
```

**Prefix vs Postfix**:
- Prefix (`++x`): Increment/decrement, then read the value
- Postfix (`x++`): Read the value, then increment/decrement

```neoc
mut i32 a = 5;
i32 b = a++;  // b = 5, a = 6 (read a, then increment)
i32 c = ++a;  // c = 7, a = 7 (increment, then read a)
```

### Operator Precedence

Operators are evaluated in the following order (highest to lowest precedence):

| Level | Operators | Description | Associativity |
|-------|-----------|-------------|---------------|
| 1 | `()` | Parentheses | N/A |
| 2 | `++` `--` `!` `-` (unary) | Unary operators | Right to left |
| 3 | `*` `/` `%` | Multiplicative | Left to right |
| 4 | `+` `-` | Additive | Left to right |
| 5 | `<` `<=` `>` `>=` | Relational | Left to right |
| 6 | `==` `!=` | Equality | Left to right |
| 7 | `&&` | Logical AND | Left to right |
| 8 | `\|\|` | Logical OR | Left to right |
| 9 | `=` `+=` `-=` `*=` `/=` `%=` | Assignment | Right to left |

**Examples**:
```neoc
i32 a = 2 + 3 * 4;           // 14 (not 20)
i32 b = (2 + 3) * 4;         // 20 (parentheses override precedence)
bool c = 5 > 3 && 2 < 4;     // true (comparison before logical AND)
i32 d = 10 - 5 - 2;          // 3 (left-to-right: (10 - 5) - 2)
```

**Best Practice**: Use parentheses to make complex expressions clear, even when not strictly necessary.

**Evaluation Order**: Operands in expressions are evaluated left-to-right.
```neoc
i32 result = func1() + func2();  // func1() called before func2()
```

**Assignment as Statement**: Assignment operators are statements, not expressions. They cannot be used within other expressions. With the notable exception of unary operators such as `++`
```neoc
mut i32 x = 5;
x = 10;  // OK: Assignment statement

mut i32 x = 5;
i32 y = x++; // OK: y is set equal to 5 and then x is incremented

// INVALID - assignment in expression
// i32 y = (x = 15);
```

**Chained Comparisons**: Comparison operators do not chain mathematically. Use logical operators instead.
```neoc
// INVALID - Does not work as expected
// bool result = (1 < 2 < 3);  // Would try to compare bool with int

// VALID
bool result = (1 < 2) && (2 < 3);
```

**Empty Blocks**: Empty code blocks are allowed but discouraged.
```neoc
if(condition) {
    // Empty block - allowed but not recommended
}
```

**Integer Literal Range**: When a literal is too large for the target type, it wraps.
```neoc
i8 x = 200;  // OK: 200 wraps to -56 in i8
u8 y = 300;  // OK: 300 wraps to 44 in u8
```

---

## Statements

A statement is a unit of execution in NeoC. All statements must end with a semicolon (`;`).

### Types of Statements

1. **Variable declaration**: Declares a new variable
2. **Assignment**: Assigns a value to an existing variable
3. **Expression**: Evaluates an expression (typically a function call)
4. **Return**: Returns from a function
5. **Control flow**: Conditionals and loops

### Examples
```neoc
i32 x = 42;              // Variable declaration statement
x = x + 1;               // Assignment statement
print(x);                // Expression statement (function call)
return x;                // Return statement
```

### Statement Terminator

Every statement must end with a semicolon (`;`). This includes statements inside blocks.

```neoc
i32 a = 1;  // Semicolon required
i32 b = 2;  // Semicolon required
return a + b;  // Semicolon required
```

**Exception**: Function and control flow definitions (the closing brace `}` terminates these).

---

## Scopes

A scope is a region of code where names (identifiers) are valid. Scopes are delimited by braces (`{` and `}`).

### Block Scopes

A block scope is created by a pair of braces. Variables declared inside a block are only visible within that block.

```neoc
fn example() {
    i32 outer = 1;
    {
        i32 inner = 2;
        // Both outer and inner are visible here
    }
    // Only outer is visible here; inner is out of scope
}
```

### Nested Scopes

Scopes can be nested. Inner scopes can access variables from outer scopes, but not vice versa.

```neoc
fn nested() {
    i32 x = 1;
    {
        i32 y = 2;
        {
            i32 z = 3;
            // x, y, and z are all visible here
        }
        // x and y are visible; z is not
    }
    // Only x is visible
}
```

### Shadowing

A variable in an inner scope can have the same name as a variable in an outer scope. This is called shadowing. The inner variable "shadows" the outer one within its scope.

```neoc
fn shadowing() {
    i32 x = 1;
    {
        i32 x = 2;  // Shadows outer x
        // x is 2 here
    }
    // x is 1 here
}
```

**Type Change in Shadowing**: Shadowed variables can have any type, independent of the outer variable's type.
```neoc
fn shadowing() {
    i32 x = 42;
    {
        bool x = true;  // OK: Different type allowed
    }
}
```

### Parentheses for Expressions

Parentheses `()` are used to group expressions and for function parameters. They do not create new scopes.

```neoc
i32 result = (2 + 3) * 4;  // Parentheses for grouping
fn Add(i32 a, i32 b) i32   // Parentheses for parameters
```

---

## Variables

Variables are named storage locations that hold values.

### Declaration

Variables are declared using the following syntax:

```
[mut] <type> <identifier> = <initial_value>;
```

- `mut` (optional): Makes the variable mutable
- `<type>`: The type of the variable
- `<identifier>`: The name of the variable
- `<initial_value>`: Every variable must be initialized

### Immutable Variables (Default)

By default, variables are immutable (cannot be changed after initialization).

```neoc
i32 x = 42;
// x = 43;  // ERROR: Cannot assign to immutable variable
```

### Mutable Variables

Variables marked with `mut` can be reassigned.

```neoc
mut i32 x = 42;
x = 43;  // OK: x is mutable
x += 10; // OK: compound assignment is also allowed
```

### Initialization

All variables must be initialized when declared. Uninitialized variables are not allowed.

```neoc
i32 x = 10;      // OK: Initialized
// i32 y;        // ERROR: Not initialized
```

### Type Specification

Variable types must be explicitly specified when declaring variables.

```neoc
i32 integer = 42;
f64 floating = 3.14;
bool flag = true;
char letter = 'A';
```

### Type Conversions

NeoC allows implicit type conversions when they are lossless. The type of an expression is determined by:
1. **Assignment context**: Expression evaluated in the type of the target variable
2. **Function argument context**: Expression evaluated in the type of the parameter
3. **No context**: Expression evaluated in the widest type of the operands

Type conversions apply automatically for function arguments following the same lossless conversion rules.

**Lossless conversions** (allowed automatically):
- Smaller integer types to larger integer types of the same signedness: `u8` → `u16` → `u32` → `u64` → `u128`
- Smaller signed integers to larger signed integers: `i8` → `i16` → `i32` → `i64` → `i128`
- Unsigned integers to signed integers of greater bit width: `u8` → `i16`, `u16` → `i32`, etc.
- Smaller floats to larger floats: `f32` → `f64`
- Integer types to float types with sufficient precision

**Examples**:
```neoc
u8 a = 10;
u8 b = 20;
i32 result = a + b;  // OK: Expression evaluated as i32, no overflow

u8 maxU8 = 255;
i16 no_overflow = max_u8 + 1;  // OK: Expression evaluated as i16 (result = 256)

u16 small = 100;
u64 large = small;   // OK: u16 fits losslessly in u64

i8 signedSmall = 5;
i64 signed_large = signed_small;  // OK: i8 fits losslessly in i64

f32 floatSmall = 1.5;
f64 float_large = float_small;    // OK: f32 fits losslessly in f64

// Function argument conversions
fn takesI32(i32 x) { return; }
u8 small = 10;
takesI32(small);  // OK: u8 converts losslessly to i32

// Boolean arithmetic (bool treated as 1-bit integer)
bool flag = true;
i32 result = 10 + flag;  // OK: result = 11 (true = 1, false = 0)
i32 x = flag;  // OK: x = 1
```

**Lossy conversions** (not allowed implicitly, require explicit cast):
- Larger types to smaller types
- Signed to unsigned (can lose sign information)
- Unsigned to signed of the same size (can overflow for large values)
- Float to integer (loses decimal part)

```neoc
i64 big = 1000;
// i32 small = big;     // ERROR: Potentially lossy conversion
i32 small = (i32)big;   // OK: Explicit cast

i32 negative = -5;
// u32 positive = negative;  // ERROR: Would lose sign information
u32 positive = (u32)negative;  // OK: Explicit cast

u32 large_unsigned = 3000000000;
// i32 as_signed = large_unsigned;  // ERROR: Would overflow i32
i32 as_signed = (i32)large_unsigned;  // OK: Explicit cast
```

**Type Casting**: Explicit type casts use C-style syntax: `(type)expression`
```neoc
f64 pi = 3.14159;
i32 truncated = (i32)pi;  // truncated = 3

i32 large = 1000;
i8 small = (i8)large;  // small = -24 (wraps)
```

**Mixed signedness in comparisons**:
Comparing signed and unsigned values of the same size is allowed but requires care. The signed value is converted to unsigned for comparison, which can produce unexpected results with negative numbers.

```neoc
i32 negative = -1;
u32 positive = 1;
bool result = negative < positive;  // Allowed, but negative is treated as large unsigned value
// It's recommended to explicitly cast for clarity in such cases
```

### Examples

```neoc
// Immutable variables
i32 count = 0;
f64 pi = 3.14159;
bool isActive = true;
char grade = 'A';

// Mutable variables
mut i32 counter = 0;
mut f32 temperature = 20.5;
mut bool running = false;

// Using variables
counter = counter + 1;
temperature += 2.5;
running = !running;
```

---

## Functions

Functions are named blocks of reusable code that can accept parameters and return values.

### Function Definition

Functions are defined using the following syntax:

```
fn <identifier>(<parameters>) [return_type] {
    <statements>
}
```

- `fn`: Function keyword
- `<identifier>`: Function name (follows identifier rules)
- `<parameters>`: Comma-separated list of parameters (can be empty)
- `[return_type]`: Optional return type (omit for void functions)
- `<statements>`: Function body

### Parameter Syntax

Parameters are declared as: `<type> <identifier>`

Multiple parameters are separated by commas:

```neoc
fn Add(i32 a, i32 b) i32 {
    return a + b;
}
```

### Return Type

If a function returns a value, the return type is specified after the parameter list:

```neoc
fn get() i32 {
    return 42;
}
```

If a function doesn't return a value, omit the return type:

```neoc
fn blank() {
    // Function body
    return;
}
```

### Return Statement

The `return` statement exits the function and optionally returns a value.

For functions with a return type:
```neoc
fn double(i32 x) i32 {
    return x * 2;  // Must return a value
}
```

For void functions (no return type):
```neoc
fn do_something() {
    print("Hello");
    // Return statement is optional at end of function
}

fn early_exit(bool condition) {
    if(condition) {
        return;  // Can return early
    }
    print("Condition was false");
}
```

**Important**:
- Functions with a return type **must** return a value on all code paths
- Void functions (no return type) do not require a return statement at the end, but can use `return;` to exit early

**All Paths Must Return**: Every possible execution path through a function with a return type must end in a return statement.

```neoc
// INVALID - Not all paths return
fn bad(i32 x) i32 {
    if(x > 0) {
        return x;
    }
    // ERROR: No return if x <= 0
}

// VALID - All paths return
fn good(i32 x) i32 {
    if(x > 0) {
        return x;
    } else {
        return 0;
    }
}

// ALSO VALID
fn also_good(i32 x) i32 {
    if(x > 0) {
        return x;
    }
    return 0;  // Default path returns
}
```

**Void Functions in Expressions**: Void functions cannot be used in expressions or assignments.
```neoc
fn void_func() { return; }

void_func();  // OK: Called as statement
// i32 x = void_func();  // ERROR: Cannot use void function result
```

**Function Call Order**: Functions can call any other function in the same source file, regardless of definition order. Forward declarations are not required.
```neoc
fn first() {
    second();  // OK: Can call second before it's defined
}

fn second() {
    first();  // OK: Mutual recursion allowed
}
```

**Recursion**: Recursive function calls are fully supported.
```neoc
fn factorial(i32 n) i32 {
    if(n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);  // Recursive call
}
```

### Function Calls

Functions are called by their name followed by parentheses containing arguments:

```neoc
main();                    // No arguments
i32 sum = add(10, 20);     // Two arguments
f64 result = calculate(x, y, z);  // Multiple arguments
```

### Examples

```neoc
// Function with no parameters, no return value
fn main() {
    return;
}

// Function with parameters and return value
fn add(i32 a, i32 b) i32 {
    return a + b;
}

// Function with multiple parameters
fn calculate(i32 x, i32 y, i32 z) i32 {
    i32 result = (x + y) * z;
    return result;
}

// Function calling other functions
fn example() {
    i32 sum = add(5, 3);
    i32 final = calculate(sum, 2, 4);
    return;
}
```

---

## Control Flow

Control flow statements determine the order in which code is executed.

### If Statements

The `if` statement executes code conditionally based on a boolean expression.

**Syntax**:
```
if(<condition>) {
    <statements>
}
```

**Example**:
```neoc
if(x > 0) {
    i32 positive = x;
}
```

### If-Else Statements

The `else` clause provides an alternative branch when the condition is false.

**Syntax**:
```
if(<condition>) {
    <statements>
} else {
    <statements>
}
```

**Example**:
```neoc
if(x > 0) {
    i32 positive = x;
} else {
    i32 nonPositive = x;
}
```

### Else-If Chains

Multiple conditions can be chained using `else if`.

**Syntax**:
```
if(<condition1>) {
    <statements>
} else if(<condition2>) {
    <statements>
} else {
    <statements>
}
```

**Example**:
```neoc
if(x > 0) {
    i32 sign = 1;
} else if(x < 0) {
    i32 sign = -1;
} else {
    i32 sign = 0;
}
```

### Condition Requirements

The condition in an `if` statement must:
1. Be enclosed in parentheses `()`
2. Evaluate to a `bool` type
3. Be a valid expression

```neoc
// Valid conditions
if(true) { }
if(x == 5) { }
if(x > 0 && y < 10) { }
if(!flag) { }

// Invalid conditions
// if(5) { }           // ERROR: Not a boolean
// if x > 0 { }        // ERROR: Missing parentheses
```

### Nested If Statements

If statements can be nested within each other:

```neoc
if(x > 0) {
    if(y > 0) {
        i32 both_positive = 1;
    }
}
```

### Complete Example

```neoc
fn check_value(i32 value) i32 {
    if(value > 100) {
        return 1;
    } else if(value > 50) {
        return 0;
    } else if(value > 0) {
        return -1;
    } else {
        return -2;
    }
}
```

---

## Example Programs

### Entry Point Requirements

Programs may optionally define an entry point function called `main` (lowercase). When present, it must return an `i32` value representing the program's exit code.

```neoc
// Entry point must be lowercase for it to work properly
fn main() i32 {
    return 0;
}
```

Programs without a `main` function are valid (useful for libraries).

### Complete Example Program

Here is a complete example program demonstrating various NeoC features:

```neoc
/// Entry point of the program
fn main() i32 {
    i32 result = fibonacci(10);

    if (is_prime(abs(result))) {
            return 1;
    } else {
        return 0;
    }
}

/// Calculates the nth Fibonacci number
fn fibonacci(i32 n) i32 {
    if(n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

/// Determines if a number is prime
fn is_prime(i32 n) bool {
    if(n <= 1) {
        return false;
    }

    mut i32 i = 2;
    if(i * i <= n) {
        if(n % i == 0) {
            return false;
        }
        i++;
    }

    return true;
}

/// Calculates the absolute value of a number
fn abs(i32 x) i32 {
    if(x < 0) {
        return -x;
    } else {
        return x;
    }
}
```

---

**End of Specification**

*NeoC Language Specification v0.0.1-DEV*
