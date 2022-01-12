/* Ofront+ 1.0 -sx3 -22 */
#include "SYSTEM.oh"




export void MK90_CLS (void);
export void MK90_PIXEL (void);
export void MK90_PSET (SHORTINT x, SHORTINT y);


/*============================================================================*/

void MK90_CLS (void)
{

  asm("\
    MOV  $0x200, R0   \n\
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
    MOV  %0, R0       \n\
    MOV  %1, R1       \n\
    JSR  PC, _MK90_PIXEL    \n\
    BIS  R2, 0x200(R1)  "
  ::"g"(x), "g"(y):"r2"
  );
}

/*----------------------------------------------------------------------------*/
