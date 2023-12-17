.data
  var_a:          .space      128


.text
main:
  la    $a0, var_a            # load address of variable to $a0
  la    $a1, 128              # load size of space to $a0, 128 is predetermined
  li    $v0, 8                # specify read string service
  syscall                     # perform

  move  $a2, $a0
  jal adm_check_type

  addi   $a0, $v0, 0    # load address of print heading
  li   $v0, 1           # specify Print integer service
  syscall               # print heading

  li   $v0, 10          # system call for exit
  syscall               # we are out of here.

adm_check_type:
  ########################
  ##
  ## THIS IS A HELPER FUNCTION TO DETERMINE IF IT IS INTEGER OR STRING
  ##
  ## PARAMETER: $a2 -> address of the variable
  ## RETURN: $v0 -> 0 if integer, 1 if string.
  ##
  ########################

  lbu $s1, 0($a2)      
  beq $s1, 10, adm_is_int 

  blt $s1, 48, adm_is_string 
  bgt $s1, 57, adm_is_string 

  addi $a2, $a2, 1        
  j adm_check_type

adm_is_int:
  li $v0, 0
  jr $ra

adm_is_string:
  li $v0, 1
  jr $ra
  

