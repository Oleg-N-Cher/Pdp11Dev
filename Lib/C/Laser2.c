void *Laser2_spradr;

void Laser2_PTBL (int x, int y, int spn)
{ /* R0 - screen address
     R2 - sprite address */
  asm("\
        MOV   %1, R0     // y     \n\
        MOV   R0, R1              \n\
        ASL   R0                  \n\
        ASL   R0                  \n\
        ADD   R1, R0              \n\
        SWAB  R0        // * 640  \n\
        ROR   R0                  \n\
.globl PTBL_E                     \n\
PTBL_E: ADD   $041000, R0         \n\
        ADD   %0, R0              \n\
        ADD   %0, R0    // + 2*x  \n\
        MOVB  %2, R1    // spn    \n\
        MOV   _Laser2_spradr, R2  \n\
FIND_S: CMPB  (R2)+, R1           \n\
        BEQ   SPRFND              \n\
        MOVB  (R2)+, R3           \n\
        BIC   $0177400, R3        \n\
        ASL   R3                  \n\
        ADD   R3, R2              \n\
        BR    FIND_S              \n\
SPRFND: TSTB  (R2)+               \n\
        MOVB  (R2)+, R3           \n\
        MOVB  (R2)+, R4           \n\
        MOV   R5, -(SP)           \n\
OUTLIN: MOV   R0, R1              \n\
        MOV   R3, R5              \n\
OUTWRD: MOV   (R2)+, (R1)+        \n\
        SOB   R5, OUTWRD          \n\
        ADD   $0100, R0           \n\
        SOB   R4, OUTLIN          \n\
        MOV   (SP)+, R5           \n"
      ::"g"(x), "g"(y), "g"(spn)
      :"r2", "r3", "r4"
  );
} // PTBL

/*------------------------------- Cut here --------------------------------*/
void Laser2_PTBLy (int x, int y, int spn)
{
  asm("\
        MOV   %1, R0     // y     \n\
        SWAB  R0                  \n\
        ROR   R0         // * 64  \n\
        ASR   R0                  \n\
        JMP   PTBL_E              \n"
      ::"g"(x), "g"(y), "g"(spn)
      :"r2", "r3", "r4"
  );
} // PTBLy

/*------------------------------- Cut here --------------------------------*/
int _colors;

void Laser2_PTBLm (int x, int y, int spn)
{ /* R0 - screen address, R1 - temp screen address
     R2 - sprite address */
  asm("\
        MOV   %1, R0     // y     \n\
        MOV   R0, R1              \n\
        ASL   R0                  \n\
        ASL   R0                  \n\
        ADD   R1, R0              \n\
        SWAB  R0        // * 640  \n\
        ROR   R0                  \n\
        ADD   $041000, R0         \n\
        ADD   %0, R0              \n\
        ADD   %0, R0    // + 2*x  \n\
        MOVB  %2, R1    // spn    \n\
        MOV   _Laser2_spradr, R2  \n\
FINDS_: CMPB  (R2)+, R1           \n\
        BEQ   SP_FND              \n\
        MOVB  (R2)+, R3           \n\
        BIC   $0177400, R3        \n\
        ASL   R3                  \n\
        ADD   R3, R2              \n\
        BR    FINDS_              \n\
SP_FND: TSTB  (R2)+               \n\
        MOVB  (R2)+, R3 // length \n\
        MOVB  (R2)+, R4 // height \n\
        MOV   (R2)+, __colors     \n\
        MOV   R5, -(SP)           \n\
OUTLNE: MOV   R0, R1              \n\
        MOV   R3, R5  // length   \n\
        MOV   R4, -(SP)           \n\
OUTBYT:       //  (R2)+ => (R1)+  \n\
        MOV   R5, -(SP)           \n\
        MOV   R3, -(SP)           \n\
        MOVB  (R2)+, R3  // byte  \n\
        MOV   R2, -(SP)           \n\
        MOV   $8, R2   // 8 bits  \n\
FRMPIX: ASL   R5                  \n\
        ASL   R5                  \n\
        CLR   R4                  \n\
        RORB  R3                  \n\
        ROLB  R4                  \n\
        BISB  __colors(R4), R5    \n\
        SOB   R2, FRMPIX          \n\
        MOV   R5, (R1)+ // =>scr  \n\
        MOV   (SP)+, R2           \n\
        MOV   (SP)+, R3           \n\
        MOV   (SP)+, R5           \n\
        SOB   R5, OUTBYT          \n\
        ADD   $0100, R0           \n\
        MOV   (SP)+, R4           \n\
        SOB   R4, OUTLNE          \n\
        MOV   (SP)+, R5           \n"
      ::"g"(x), "g"(y), "g"(spn)
      :"r2", "r3", "r4"
  );
} // Laser2_PTBLm
