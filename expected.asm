.data
  to_print:  .asciiz "Jhoanna"
.text
  la   $a0, to_print       # load address of spacer for syscall
  li   $v0, 4               # specify Print String service
  syscall
  li   $v0, 10          # system call for exit
  syscall               # we are out of here.