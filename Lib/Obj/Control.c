/* Ofront+ 1.0 -sx3 -22 */
#include "SYSTEM.oh"




export CHAR Control_Get (void);


/*============================================================================*/

CHAR Control_Get (void)
{

  asm("\
    BIT  $100, @$0177716         \n\
    BNE  2$                      \n\
    MOV  $CodesK-CodesJ, R0      \n\
1$: CMPB CodesK-1(R0), @$0177662 \n\
    BEQ  4$                      \n\
    SOB  R0, 1$                  \n\
2$: MOV  $CodesK-CodesJ, R0      \n\
3$: BITB CodesJ-1(R0), @$0177714 \n\
    BNE  4$                      \n\
    SOB  R0, 3$                  \n\
4$: RTS  PC                      \n\
CodesJ: .BYTE 010, 02, 01, 04,040,0100 \n\
CodesK: .BYTE 010,031,032,033,040, 015 \n"
  );
  __RETCHK((CHAR*)"Control", 0);
}

/*----------------------------------------------------------------------------*/
