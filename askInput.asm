.data
  var_a:          .space      128
  new_line:       .asciiz     "\n"
  new_space:       .asciiz     " "

.text
main:
  la    $a0, var_a            # load address of variable to $a0
  la    $a1, 128              # load size of space to $a0, 128 is predetermined
  li    $v0, 8                # specify read string service
  syscall                     # perform

  move  $a2, $a0
  addi $v0, $v0, 0
  addi $v1, $v1, 0
  jal adm_check_type

  addi   $a0, $v1, 0    # load result integer to a0
  beq $v0, 0, adm_print_int
  la    $a0, var_a 
  bne $v0, 0 adm_print_string

continue:
  li   $v0, 10          # system call for exit
  syscall               # we are out of here.

adm_check_type:
  ########################
  ##
  ## THIS IS A HELPER FUNCTION TO DETERMINE IF IT IS INTEGER OR STRING
  ##
  ## PARAMETER: $a2 -> address of the variable
  ## RETURN:    $v0 -> 0 if integer, 1 if string.
  ##            $v1 -> the integer
  ########################

  addi $s0, $zero, 10   # multiplier of the integer

  lbu $s1, 0($a2)      
  beq $s1, 10, adm_is_int 

  blt $s1, 48, adm_is_string 
  bgt $s1, 57, adm_is_string 

  addi $s1, $s1, -48    #converts s1's ascii value to dec value

  mul $v1, $v1, $s0     # sum *= 10
  add $v1, $v1, $s1     # sum += array[s1]-'0'

  addi $a2, $a2, 1        
  j adm_check_type

adm_is_int:
  li $v0, 0
  jr $ra

adm_is_string:
  li   $v1, 0           # no integer equivalent since string
  li $v0, 1
  jr $ra
  

adm_print_int:
  ########################
  ##
  ## THIS IS A HELPER FUNCTION TO PRINT INTEGER
  ##
  ## PARAMETER: $a0 -> integer to Print
  ##
  ########################
  
  li   $v0, 1           # specify Print integer service
  syscall               # print heading
  j continue

adm_print_string:
  ########################
  ##
  ## THIS IS A HELPER FUNCTION TO PRINT STRING
  ##
  ## PARAMETER: $a0 -> address of string to Print
  ##
  ########################

  move $s2, $a0
  jal adm_remove_last_new_line
  move $a0, $s2
  li   $v0, 4           # specify Print integer service
  syscall               # print heading
  j continue

adm_remove_last_new_line:
  lbu $s1, 0($a0)      
  beq $s1, 10, exit_adm_remove_last_new_line
  addi $a0, $a0, 1        
  j adm_remove_last_new_line
  

exit_adm_remove_last_new_line:
  li $t0, 0
  sb $t0, 0($a0)
  jr $ra

