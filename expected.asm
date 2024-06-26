.data
  hello:      .asciiz     "Hello world!"
  new_line:   .asciiz     "\n"
  result:     .word       0
  a:          .space      128
  b:          .word       2

.text

main:
  la   $a0, a           # address
  la   $a1, 128         # size
  li $v0, 8             # specify read integer service
  syscall

  # Initialize $s0 to 0 (assume it's an integer)
  li $s0, 0
  move $a3, $a0
  j check_type

print_res:              # 1 -> string, 0 -> integer
  addi   $a0, $s0, 0    # load address of print heading
  li   $v0, 1           # specify Print integer service
  syscall               # print heading
  j exit
  
check_type:
  # Load the first character from the buffer
  lbu $s1, 0($a3)

  # jal print_char

  beq $s1, 10, print_res     # 10 -> ASCII code for '\n'

  # Check if the first character is a digit
  blt $s1, 48, is_string      # 48 -> ASCII code for '0'
  bgt $s1, 57, is_string      # 57 -> ASCII code for '9'

  # The first character is a digit so iterate
  addi $a3, $a3, 1
  j check_type

is_string:
  li $s0, 1
  j print_res

print_char:
  move   $a0, $s1        # load address of print heading
  li   $v0, 11           # specify Print Character service
  syscall                # print heading
  
  la   $a0, new_line    # load address of print heading
  li   $v0, 4           # specify Print String service
  syscall               # print heading
  jr $ra

exit:
  li   $v0, 10          # system call for exit
  syscall               # we are out of here.