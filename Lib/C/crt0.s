; 
     .TITLE CRT0 shim
        .IDENT "V00.00"

        .text
        .GLOBAL _main
        .GLOBAL start
start:
        .GLOBAL ___main
        jmp     _main
___main:
        rts   pc
        .even
        // .data
        .end   
