; 
     .TITLE CRT0 shim
        .IDENT "V00.00"
        .text

        .GLOBAL _main
        .GLOBAL start
start:
        NOP         // obligatory!
        MTPS $0xE0  // disable interrupts

        .GLOBAL ___main
        jmp     _main
___main:

        rts   pc
        .even
        // .data
        .end   
