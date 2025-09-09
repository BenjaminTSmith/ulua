-- test_lexer.lua
-- Covers Lua 5.1 operators, keywords, and syntax

-- literals
a_number = 123
a_float = 3.14
a_string = "hello"
a_longstring = [[multi
line
string]]
a_bool_true = true
a_bool_false = false
a_nil = nil

-- identifiers
local identifier_test = "ok"

-- operators
local sum       = 1 + 2
local diff      = 5 - 3
local product   = 4 * 6
local quotient  = 8 / 2
local mod       = 10 % 3
local power     = 2 ^ 3
local concat    = "Lua" .. "5.1"
local len       = #"string"

-- comparisons
local eq        = (1 == 1)
local neq       = (1 ~= 2)
local leq       = (2 <= 3)
local geq       = (3 >= 2)
local lt        = (2 < 3)
local gt        = (3 > 2)

-- logical
local and_test  = true and false
local or_test   = false or true
local not_test  = not false

-- table constructor
local tbl = { 1, 2, 3; key = "value", nested = { a = 10 } }

-- dot, colon
print(tbl.key)
tbl:method_call()

-- varargs ...
function f(a, b, ...)
  print(a, b, ...)
end
f(1, 2, 3, 4)

-- control flow
if eq then
  print("if/then/else")
elseif neq then
  print("elseif branch")
else
  print("else branch")
end

-- while loop
local i = 0
while i < 3 do
  i = i + 1
end

-- repeat/until loop
repeat
  i = i - 1
until i == 0

-- for loop numeric
for j = 1, 5, 2 do
  print(j)
end

-- for loop generic
for k, v in pairs(tbl) do
  print(k, v)
end

-- function definition
function add(x, y)
  return x + y
end

-- anonymous function
local f2 = function(x) return x * 2 end

-- break statement
for n = 1, 10 do
  if n == 3 then
    break
  end
end

-- do block
do
  local scoped = "inside do block"
  print(scoped)
end

