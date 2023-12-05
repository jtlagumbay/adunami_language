# Compute first twelve Fibonacci numbers and put in array, then print
.data
      fibs: .word   0 : 12        # "array" of 12 words to contain fib values
      size: .word  12             # size of "array" 
      space:.asciiz  " "          # space to insert between numbers
      input: .asciiz  "Enter integer: "
      newLine: .asciiz "\n|
.text
      la   $a0, input           # load address of print heading
      li   $v0, 4               # specify Print String service
      syscall                   # print heading
      li   $v0, 5               # read integer
      syscall
      add $s0, $v0, $zero
      la   $a0, input           # load address of print heading
      li   $v0, 4               # specify Print String service
      syscall                   # print heading
      li   $v0, 5               # read integer
      syscall
      add $s1, $v0, $zero
      add $s2, $s0, $s1
      add $a0, $s2, $zero
      li $v0, 1
      syscall
      li $v0, 10
      syscall
