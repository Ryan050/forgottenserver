# Language conventions
The following conventions apply only to C++.

These conventions will change over time, but changing them requires a
convincing rationale.

## Language use
C++ is a complex language, and we strive to use it in a manner that is not
surprising. So, the primary rule is to be reasonable. Use a language feature in
known good ways. If you need to use a feature in an unusual way, or a way that
violates the "should" rules below, seek guidance, review and feedback from the
wider community.

### Variable definitions
Variables should be defined at the point of first use, rather than at the top
of the function.

Variables may be simultaneously defined and tested in control expressions.

### Class definitions
Use public inheritance to describe interface inheritance, i.e. 'is-a'
relationships. Use private and protected inheritance to describe implementation
inheritance. Implementation inheritance can be expedient, but think twice
before using it in code intended to last a long time.

Complex hierarchies are to be avoided. Take special care with multiple
inheritance. On the rare occasion that using mulitple inheritance is indeed
useful, prepare design rationales in advance.

Think carefully about the size and performance impact of virtual functions and
virtual bases before using them.

Prefer to make data members private.

#### Constructors and destructors
All constructors should initialize data members in the member initializer list
rather than in the body of the constructor.

A class with virtual functions or virtual bases should have a virtual
destructor.

#### Conversions
Single argument constructors should nearly always be declared explicit.

Conversion operators should be avoided.

#### Overloading Functions
Overloading functions is permitted, but take care to ensure that overloads are
not surprising, i.e. semantically identical or at least very similar. Virtual
functions should not be overloaded.

#### Overloading operators
Overloading operators is permitted, but take care to ensure that overloads are
not surprising. Some unsurprising uses are in the implementation of numeric
types and in following the C++ Standard Library's conventions. In addition,
overloaded operators, excepting the call operator, should not be used for
expensive implementations.

#### Default arguments
Default arguments are another type of function overloading, and the same rules
apply. Default arguments must always be POD values, i.e. may not run
constructors. Virtual functions should not have default arguments.

### Casts
C-style casts should not be used. Instead, use C++-style casts.

### Compiler options
The server must build cleanly with `-Wall` and `-Wextra`, as specified by
[CMakeLists.txt](CMakeLists.txt).

## Formatting conventions

### Whitespace
Tabs for indentation level and spaces for alignment. No whitespace at the
end of a line.

Unix-style linebreaks (`\n`), not Windows-style (`\r\n`).

### Line length
In general, keep lines at most 80 columns (for laptop side-by-side diffing and
two-window tiling).

### Indentation
One tab per logic level, or four spaces in Lua code.

Note that class visibility consume a logic level. See examples below.

### Control structures
Use K&R bracing style: left brace at end of first line, cuddle else on both
sides.
```cpp
if (...) {
} else if (...) {
} else {
}

while (...) {
}

do {
} while (...);

for (...; ...; ...) {
}
```

Always brace controlled statements, even a single-line consequent of an if else
else. This is redundant typically, but it avoids dangling else bugs, so it's
safer at scale than fine-tuning.

Break long conditions after && and || logical connectives. See below for the
rule for other operators.

Control keywords if, for, while, and switch are always followed by a space to
distinguish them from function calls which have no trailing space.

### Names
When structs and/or classes have member functions, prefer to name clashing data
members with a leading `_`.

Template parameter names should use CamelCase, following the C++ Standard.

### Class definitions
Indent protection labels by one level.

Indent class members by two levels.

Prefer to put the entire class head on a single line.
```cpp
class ClassName : Base {
```

Otherwise, put the colon of the base list at the end of a line.
```cpp
class ARatherLongClassName :
    WithAVeryLongBaseName, AndAnotherJustToMakeLifeHard
{
```

If the base clause exceeds one line, move overflowing initializers to the next
line and align.
```cpp
class ClassName :
    Base1<TemplateArgument1>, Base2<TemplateArgument1>,
    Base3<TemplateArgument1>, Base4<TemplateArgument1>
{
```

When defining a class,
- first define all public types
- then declare all public constructors
- then declare the public destructor
- then declare all public member functions
- then declare all public member variables
- then declare all protected types
- then declare all protected constructors
- then declare the protected destructor
- then declare all protected member functions
- then declare all protected member variables
- then declare all private types
- then declare all private constructors
- then declare the private destructor
- then declare all private member functions
- then declare all private member variables

Semantic constraints may require a different declaration order, but seek to
minimize the potential confusion.

Close a class definition with a right brace, semicolon, optional closing
comment, and a new line.

```cpp
}; // class ClassName
```

#### Class member definitions
Define all members outside the class definition. That is, there are no function
bodies or member initializers inside the class definition. It is okay to define
a member function if it has a single statement and a branch.
```cpp
        int answer() {
            return 42;
        }

        void sendAnswer() {
            if (client) {
                client->sendAnswer(answer());
            }
        }
```

Prefer to put the entire member head on a single line.
```cpp
ClassName::ClassName() : BaseClass(), member(42)
{
    ...
}
```

When that is not possible, place the colon of the initializer clause at the end
of a line.
```cpp
ClassName::ClassName() :
    Base1(), Base2(), member1(), member2(), member3(), member4()
{
    ...
}
```

If the initializer clause exceeds one line, move overflowing initializers to
the next line and align.
```cpp
ClassName::ClassName() :
    Base1(some_expression), Base2(another_expression),
    member1(my_expressions_everywhere)
{
    ...
}
```

If a C++ function name is long enough to cause the first function parameter
with its type to exceed 80 characters, it should appear on the next line
indented one level.
```cpp
void VeryLongClassName::veryLongFunctionName(
    VeryLongTypeName arg)
{
```

### Templates
A declaration following a template parameter list should not have additional
indentation.

### Namespaces
Open a namespace with the namespace name followed by a left brace and a new line.
```cpp
namespace tfstools {
```

Close a namespace with a right brace, optional closing comment, and a new line.
```cpp
} // namespace tfstools
```

Definitions within the body of a namespace are not indented.

### Expressions
Code should use the following formatting conventions:

| **for**             | **use...** | **...instead of** |
|---------------------|:----------:|:-----------------:|
| logical not         | `!x`       | `! x`             |
| bitwise complement  | `~x`       | `~ x`             |
| unary minus         | `-x`       | `- x`             |
| pointer dereference | `*x`       | `* x`             |
