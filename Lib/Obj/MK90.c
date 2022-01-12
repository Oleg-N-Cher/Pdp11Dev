/* Ofront+ 1.0 -sx3 -22 */
#include "SYSTEM.oh"




export void MK90_CLS (void);
export void MK90_LINE (SHORTINT x0, SHORTINT y0, SHORTINT x1, SHORTINT y1);
export void MK90_PIXEL (void);
export void MK90_PSET (SHORTINT x, SHORTINT y);


/*============================================================================*/

void MK90_CLS (void)
{

  asm("\
    MOV  $0x200, @$0xE800 \n\
    MOV  $0x200, R0  \n\
    MOV  $0x1E0, R1   \n\
1$: CLR  (R0)+        \n\
    SOB  R1, 1$       \n"
  );
}

/*----------------------------------------------------------------------------*/
void MK90_PIXEL (void)
{

  asm("\
    MOV  $0x4000, R2  \n\
    BIT  $0x20, R1    \n\
    BNE  0$           \n\
    SWAB R2           \n\
0$: MOV  R0, -(SP)    \n\
    BIC  $0xFFF8, R0  \n\
    NEG  R0           \n\
    INC  R0           \n\
    ASH  R0, R2       \n\
    MOV  (SP)+, R0    \n\
    ASR  R0           \n\
    ASR  R0           \n\
    BIC  $1, R0       \n\
    BIC  $0xFFE0, R1  \n\
    MUL  $0x1E, R1    \n\
    ADD  R0, R1       \n"
  );
}

/*----------------------------------------------------------------------------*/
void MK90_PSET (SHORTINT x, SHORTINT y)
{

  asm("\
    MOV  %0, R0           \n\
    MOV  %1, R1           \n\
    JSR  PC, _MK90_PIXEL  \n\
    BIS  R2, 0x200(R1)    \n"
  ::"g"(x), "g"(y):"r2"
  );
}

/*----------------------------------------------------------------------------*/
void MK90_LINE (SHORTINT x0, SHORTINT y0, SHORTINT x1, SHORTINT y1)
{

  asm("\
        MOV  %0, R0          \n\
        MOV  %1, R1          \n\
        MOV  %2, R2          \n\
        MOV  %3, R3          \n\
        MOV  R5, -(SP)       \n\
        MOV  R3, R4          \n\
        SUB  R1, R4          \n\
        BCC  LINE1           \n\
        NEG  R4              \n\
LINE1:  MOV  R2, R5          \n\
        SUB  R0, R5          \n\
        BCC  LINE2           \n\
        NEG  R5              \n\
LINE2:  SUB  R4, R5          \n\
        BPL  LINE3           \n\
        JSR  PC, SWAP01      \n\
        JSR  PC, SWAP23      \n\
LINE3:  CMP  R2, R0          \n\
        BPL  LINE4           \n\
        MOV  R0, R4          \n\
        MOV  R2, R0          \n\
        MOV  R4, R2          \n\
        MOV  R1, R4          \n\
        MOV  R3, R1          \n\
        MOV  R4, R3          \n\
LINE4:  MOV  R2, -(SP)       \n\
        SUB  R0, R2          \n\
        MOV  $1, -(SP)       \n\
        SUB  R1, R3          \n\
        BPL  LINE5           \n\
        NEG  (SP)            \n\
        NEG  R3              \n\
LINE5:  MOV  R2,R4           \n\
        ASR  R4              \n\
LINE6:  JSR  PC, SWAP1       \n\
        MOV  R0, -(SP)       \n\
        MOV  R1, -(SP)       \n\
        MOV  R2, -(SP)       \n\
        JSR  PC, _MK90_PIXEL \n\
        BIS  R2, 0x200(R1)   \n\
        MOV  (SP)+, R2       \n\
        MOV  (SP)+, R1       \n\
        MOV  (SP)+, R0       \n\
        JSR  PC, SWAP1       \n\
        SUB  R3, R4          \n\
        BPL  LINE7           \n\
        ADD  (SP), R1        \n\
        ADD  R2, R4          \n\
LINE7:  INC  R0              \n\
        CMP  2(SP), R0       \n\
        BCC  LINE6           \n\
        ADD  $4, SP          \n\
        BR   DONE            \n\
SWAP1:  TST  R5              \n\
        BPL  SWAP2           \n\
SWAP01: MOV  R0, -(SP)       \n\
        MOV  R1, R0          \n\
        MOV  (SP)+, R1       \n\
SWAP2:  RTS  PC              \n\
SWAP23: MOV  R2, -(SP)       \n\
        MOV  R3, R2          \n\
        MOV  (SP)+, R3       \n\
        RTS  PC              \n\
DONE:   MOV  (SP)+, R5       \n"
  ::"g"(x0), "g"(y0), "g"(x1), "g"(y1)
  :"r2","r3","r4"
  );
}

/*----------------------------------------------------------------------------*/
