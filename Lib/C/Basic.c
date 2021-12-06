signed char Basic_x, Basic_y;
signed char Basic_color [2];
void *Basic_scradr, *Basic_seed, *Basic_fontadr;

/*------------------------------- Cut here --------------------------------*/
void Basic_Init (void)
{
  asm("\
        CMPB  @$0177717, $0200       \n\
        BEQ   10$                    \n\
        MTPS  $0340                  \n\
        MOV   $016000, @$0177716     \n\
10$:    MOVB  $0100, @$0177660       \n\
        TSTB  @$0177662  // Get key  \n\
        MOV   $041000, _Basic_scradr \n\
        MOV   $0100000, _Basic_seed"
  );
} // Basic_Init
/*
        MOV  $0152112, @$030    \n\
        EMT  0                  \n\
        MOV  $5, R0             \n\
        EMT  012  // Scr 40000  \n\
        CLR  R0                 \n\
        EMT  056  // Buf 0 r/w  \n\
        MOV  $0140000, @$4      \n\
*/

/*------------------------------- Cut here --------------------------------*/
void Basic_Quit (void)
{
  asm("\
        CLRB  @$0177660           \n"
  );
} // Basic_Quit

/*------------------------------- Cut here --------------------------------*/
void Basic_AT (int x, int y)
{
  asm("\
        MOV   %1, R0    // y      \n\
        MOV   R0, R1              \n\
        ASL   R0                  \n\
        ASL   R0                  \n\
        ADD   R1, R0              \n\
        SWAB  R0        // * 640  \n\
        ROR   R0                  \n\
        ADD   $041000, R0         \n\
        ADD   %0, R0              \n\
        ADD   %0, R0    // + 2*x  \n\
        MOV   R0, _Basic_scradr   \n"
      ::"g"(x),"g"(y)
  );
} // Basic_AT

/*------------------------------- Cut here --------------------------------*/
void Basic_CLS (void)  // Clear screen
{
  asm("\
        MOV  $040000, R0        \n\
0$:     CLRB 037777(R0)         \n\
        SOB  R0, 0$             \n\
        MOV  $01330, @$0177664  \n"  // Set default scroll position
  );
} // Basic_CLS

/*------------------------------- Cut here --------------------------------*/
signed char Basic_INKEY (void) {
  asm("\
        CLR  R0                 \n\
        TSTB @$0177660          \n\
        BPL  NOKEY$             \n\
        MOVB @$0177662, R0      \n\
 NOKEY$:                        \n"
  );
} // Basic_INKEY

/*------------------------------- Cut here --------------------------------*/
void Basic_LINE (int x, int y)
{
  asm("\
        MOVB _Basic_color, R0   \n\
        MOV  %0, R1             \n\
        MOV  %1, R2             \n\
        EMT  032                \n"
      ::"g"(x),"g"(y):"r2"
  );
} // Basic_LINE

/*------------------------------- Cut here --------------------------------*/
void Basic_PALETTE (int n)
{
  asm("\
        CMPB @$0177717, $0200   \n\
        BEQ  BK0010             \n\
        MOV  %0, R0             \n\
        BIC  $0177760, R0       \n\
        SWAB R0                 \n\
        BIS  $040000, R0        \n\
        MOV  R0, @$0177662      \n\
BK0010:"
      ::"g"(n)
  );
} // Basic_PALETTE

/*------------------------------- Cut here --------------------------------*/
void Basic_PRCHAR (signed char ch)
{
  asm("\
        MOVB  %0, R0                \n\
        MOV   R0, R1                \n\
        ASL   R0                    \n\
        ASL   R0         // * 10    \n\
        ADD   R1, R0                \n\
        ASL   R0                    \n\
        ADD   _Basic_fontadr, R0    \n\
        MOV   _Basic_scradr, R1     \n\
        MOV   R1, R2                \n\
        ADD   $2, R2                \n\
        MOV   R2, _Basic_scradr     \n\
        MOV   $10, R3   // 10 lines \n\
        MOV   R5, -(SP)             \n\
OUTLIN: MOV   R3, -(SP)             \n\
        MOVB  (R0)+, R3 // fnt byte \n\
        MOV   $8, R2    // 8 bits   \n\
FRMPIX: ASL   R5                    \n\
        ASL   R5                    \n\
        CLR   R4                    \n\
        RORB  R3                    \n\
        ROLB  R4                    \n\
        BISB  _Basic_color(R4), R5  \n\
        SOB   R2, FRMPIX            \n\
        MOV   R5, @R1  // => screen \n\
        ADD   $0100, R1 // nxt line \n\
        MOV   (SP)+, R3             \n\
        SOB   R3, OUTLIN            \n\
        MOV   (SP)+, R5             \n"
      ::"g"(ch):"r2","r3","r4"
  );
} // Basic_PRCHAR

/*------------------------------- Cut here --------------------------------*/
void Basic_PRINT (int n)
{
  asm("\
        MOV   %0, R4                \n\
        MOV   $DIGIT$, R2           \n\
        MOV   $5, R3                \n\
1$:     MOV   $057, R0              \n\
2$:     INC   R0                    \n\
        SUB   @R2, R4               \n\
        BHIS  2$                    \n\
        MOVB  R0, -(SP)             \n\
        JSR   PC, _Basic_PRCHAR     \n\
        ADD   $2, SP                \n\
        ADD   (R2)+, R4             \n\
        SOB   R3, 1$                \n\
        BR    3$                    \n\
DIGIT$: .WORD 10000,1000,100,10,1   \n\
3$:                                 \n"
      ::"g"(n):"r2","r3","r4"
  );
} // Basic_PRINT

/*------------------------------- Cut here --------------------------------*/
void Basic_PRSTR (char *str)
{
  asm("\
        MOV   %0, R0                \n\
NEXTCH: TSTB  @R0                   \n\
        BEQ   NULLCH                \n\
        MOV   R0, -(SP)             \n\
        MOVB  @R0, -(SP)            \n\
        JSR   PC, _Basic_PRCHAR     \n\
        ADD   $2, SP                \n\
        MOV   (SP)+, R0             \n\
        INC   R0                    \n\
        BR    NEXTCH                \n\
NULLCH:"
      ::"g"(str)
  );
} // Basic_PRSTR

/*------------------------------- Cut here --------------------------------*/
void Basic_PSET (int x, int y)
{
  asm("\
        MOVB _Basic_color, R0   \n\
        MOV  %0, R1             \n\
        MOV  %1, R2             \n\
        EMT  030                \n"
      ::"g"(x),"g"(y):"r2"
  );
} // Basic_PSET

/*------------------------------- Cut here --------------------------------*/
void Basic_PUT (void *spradr)
{ /* R0 - screen address
     R2 - sprite address */
  asm("\
        MOV   _Basic_scradr, R0   \n\
        MOV   %0, R2   // spradr  \n\
        MOVB  (R2)+, R3           \n\
        MOVB  (R2)+, R4           \n\
        MOV   R5, -(SP)           \n\
OUTROW: MOV   R0, R1              \n\
        MOV   R3, R5              \n\
OUTWRD: MOV   (R2)+, (R1)+        \n\
        SOB   R5, OUTWRD          \n\
        ADD   $0100, R0           \n\
        SOB   R4, OUTROW          \n\
        MOV   (SP)+, R5           \n"
      ::"g"(spradr)
      :"r2","r3","r4"
  );
} // Basic_PUT

/*------------------------------- Cut here --------------------------------*/
int _seed1 = 0173451, _seed2 = 54102;

signed char Basic_RNDN (void)
{
  // Random generator for BK-0010 (c) Manwe/SandS 2019
  // https://zx-pk.ru/threads/11381-napisanie-programm-dlya-bk0010.html?p=1002531#post1002531
  asm("\
        MOV   __seed1, R0  \n\
        MOV   __seed2, R1  \n\
        ROR   R0           \n\
        ROL   R1           \n\
        SWAB  R1           \n\
        XOR   R1, R0       \n\
        MOV   R0, __seed1  \n\
        MOV   R1, __seed2  \n\
.globl  _Basic_RANDOMIZE   \n\
_Basic_RANDOMIZE:          \n"
  );
} // Basic_RNDN

/*------------------------------- Cut here --------------------------------*/
signed char Basic_SGN (signed char x)
  /*if (x < 0) return -1;
  if (x == 0) return 0;
  return 1;*/
{
  asm("\
        TSTB  %0           \n\
        BGT   ONE$         \n\
        SXT   R0           \n\
        RTS   PC           \n\
ONE$:   MOVB  $1, R0       \n"
      ::"g"(x)
  );
} // Basic_SGN