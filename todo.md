# Todo

## Variable
-    Field (variable)
-    C_str ?
-    Table
-    Array of table

### Values must have one of the following types.

| types           | status |
|-----------------|--------|
|String           |       x|
|Integer          |       x|
|Float            |       x|
|Boolean          |       x|
|Offset Date-Time |       x|
|Local Date-Time  |       x|
|Local Date       |       x|
|Local Time       |       x|
|Array            |       x|
|Inline Table     |       x|
#### Unspecified values are invalid.

[//]: ✓x

## error handeling
-    key overlaping on table
-    quote management  ' "


```c
struct Table
{
    char*   tableName;
    Field*  fields;
    size_t  fieldAmount;
}

struct Field
{
    char*       key;
    void*       value;
    size_t      amount;
    Variable    type;
}
```