.data
  hello: .asciiz "Hello world!"
.text
  la   $a0, hello        # load address of print heading
  li   $v0, 4           # specify Print String service
  syscall               # print heading
  li   $v0, 10          # system call for exit
  syscall               # we are out of here.