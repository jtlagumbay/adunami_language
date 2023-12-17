.data
  var_a:          .space      128


.text
main:
  la    $a0, var_a            # load address of variable to $a0
  la    $a1, 128              # load size of space to $a0, 128 is predetermined
  li    $v0, 8                # specify read string service
  syscall                     # perform

  li    $v0, 0                 # assume its integer
  move  $a2, $a0
  jal adm_check_type

  addi   $a0, $v0, 0    # load address of print heading
  li   $v0, 1           # specify Print integer service
  syscall               # print heading

  li   $v0, 10          # system call for exit
  syscall               # we are out of here.

adm_check_type:
  lbu $s1, 0($a2)             # Load the first character from the buffer 
  beq $s1, 10, adm_print_res      # 10 -> ASCII code for '\n'

  # Check if the first character is a digit
  blt $s1, 48, adm_is_string      # 48 -> ASCII code for '0'
  bgt $s1, 57, adm_is_string      # 57 -> ASCII code for '9'

  # The first character is a digit so iterate
  addi $a2, $a2, 1            # move to next character
  j check_type

adm_print_res:              # 1 -> string, 0 -> integer
  jr $ra

adm_is_string:
  li $v0, 1
  jr $ra
  

