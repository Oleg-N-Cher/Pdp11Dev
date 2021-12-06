; 
     .TITLE CRT0RT shim
        .IDENT "V00.00"

        # STACK = 0x1000
        STACK = 01000

        .text
        .GLOBAL _main
        .GLOBAL start
start:
        mov   $STACK, sp
//      mov   $0, -(sp)    // env
//      mov   args, -(sp)  // args
//      mov   $1, -(sp)    // argc
        jsr   pc, _main
//      add   $6, sp
        emt   0350         // .EXIT - Terminate Program
        nop

        .GLOBAL ___main
___main:
        rts   pc
        .even
        // .data
$__progname:
//      .asciz  "hello.rom"

args:
//      .word $__progname
//      .word 0

        .end   
