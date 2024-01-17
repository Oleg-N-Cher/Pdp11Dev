void *Laser2_spradr;

void Laser2_PTND (int x, int y, int spn)
{
  asm("\
        MOVB  $0304, R1  // BICB  \n\
        JMP   PTBLMODE            \n"
      ::"g"(x), "g"(y), "g"(spn)
  );
} // PTND

/*------------------------------- Cut here --------------------------------*/
void Laser2_PTOR (int x, int y, int spn)
{
  asm("\
        MOVB  $0324, R1  // BISB  \n\
        JMP   PTBLMODE            \n"
      ::"g"(x), "g"(y), "g"(spn)
  );
} // PTOR

/*------------------------------- Cut here --------------------------------*/
void Laser2_PTBL (int x, int y, int spn)
{ /* R0 - screen address
     R2 - sprite address */
  asm("\
        MOVB  $0224, R1  // MOVB  \n\
.globl PTBLMODE                   \n\
PTBLMODE:                         \n\
        MOV   %1, R0     // y     \n\
        ASL   R0                  \n\
        ASL   R0                  \n\
        ADD   %1, R0              \n\
        SWAB  R0        // * 640  \n\
        ROR   R0                  \n\
.globl PTBL_E                     \n\
PTBL_E: MOVB  R1, OUTWRD+1        \n\
        MOVB  R1, OUTWRD2+1       \n\
        ADD   $041000, R0         \n\
        ADD   %0, R0              \n\
        ADD   %0, R0    // + 2*x  \n\
        MOVB  %2, R1    // spn    \n\
        MOV   R2, -(SP)           \n\
        MOV   R3, -(SP)           \n\
        MOV   R4, -(SP)           \n\
        MOV   _Laser2_spradr, R2  \n\
FIND_S: CMPB  (R2)+, R1           \n\
        BEQ   SPRFND              \n\
        MOVB  (R2)+, R3           \n\
        BIC   $0177400, R3        \n\
        ADD   R3, R2              \n\
        BR    FIND_S              \n\
SPRFND: TSTB  (R2)+               \n\
        MOVB  (R2)+, R3 // length \n\
        MOVB  (R2)+, R4 // height \n\
        MOV   R5, -(SP)           \n\
OUTLIN: MOV   R0, R1              \n\
        MOV   R3, R5              \n\
OUTWRD: MOVB  (R2)+, (R1)+        \n\
OUTWRD2:MOVB  (R2)+, (R1)+        \n\
        SOB   R5, OUTWRD          \n\
        ADD   $0100, R0           \n\
        SOB   R4, OUTLIN          \n\
        MOV   (SP)+, R5           \n\
        MOV   (SP)+, R4           \n\
        MOV   (SP)+, R3           \n\
        MOV   (SP)+, R2           \n"
      ::"g"(x), "g"(y), "g"(spn)
  );
} // PTBL

/*------------------------------- Cut here --------------------------------*/
void Laser2_PTBLy (int x, int y, int spn)
{
  asm("\
        MOVB  $0224, R1  // MOVB  \n\
        MOV   %1, R0     // y     \n\
        SWAB  R0                  \n\
        ROR   R0         // * 64  \n\
        ASR   R0                  \n\
        JMP   PTBL_E              \n"
      ::"g"(x), "g"(y), "g"(spn)
  );
} // PTBLy

/*------------------------------- Cut here --------------------------------*/
int _colors;

void Laser2_PTBLm (int x, int y, int spn)
{ /* R0 - screen address, R1 - temp screen address
     R2 - sprite address */
  asm("\
        MOV   %1, R0     // y     \n\
        ASL   R0                  \n\
        ASL   R0                  \n\
        ADD   %1, R0              \n\
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
        ADD   R3, R2              \n\
        BR    FINDS_              \n\
SP_FND: TSTB  (R2)+               \n\
        MOVB  (R2)+, R3 // length \n\
        MOVB  (R2)+, R4 // height \n\
        MOVB  @R2, R1   // colors \n\
        ASL   R1                  \n\
        ASL   R1                  \n\
        BISB  (R2)+, R1           \n\
        BIC   $0176374, R1        \n\
        MOV   R1, __colors        \n\
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

/*------------------------------- Cut here --------------------------------*/
void _Laser2_XYtoScr (int dummy, int r2, int r3,
  int x, int y, int len, int hgt)
{ /* R0 - screen address */
  asm("\
        MOV   %4, R0  // y      \n\
        ASL   R0                \n\
        ASL   R0                \n\
        ADD   %4, R0            \n\
        SWAB  R0      // * 640  \n\
        ROR   R0                \n\
        ADD   $041000, R0       \n\
        ADD   %3, R0            \n\
        ADD   %3, R0  // + 2*x  \n\
        MOV   %5, R2  // len    \n\
        MOV   %6, R3  // hgt    \n"
      ::"g"(dummy), "g"(r2), "g"(r3),
        "g"(x), "g"(y), "g"(len), "g"(hgt)
  );
} // _Laser2_XYtoScr

/*------------------------------- Cut here --------------------------------*/
void Laser2_CLSV (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        MOV   R4, -(SP)             \n\
OUTLN2: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
OUTWR2: CLR   (R1)+                 \n\
        SOB   R4, OUTWR2            \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN2            \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_CLSV

/*------------------------------- Cut here --------------------------------*/
void Laser2_SETV (int x, int y, int len, int hgt, unsigned char pattern)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        MOV   R4, -(SP)             \n\
OUTLNF: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
OUTWRF: MOVB  16(SP), (R1)+         \n\
        SOB   R4, OUTWRF            \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLNF            \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_SETV

/*------------------------------- Cut here --------------------------------*/
void _MIRROR_BYTE (void)
{ // (c) Serg @ t.me/bk0010_11m
  asm("\
// Input: R1 byte; Output: R3 byte  \n\
        JSR   PC, (PC)              \n\
        JSR   PC, (PC)              \n\
        ROLB  R1                    \n\
        ROLB  R1                    \n\
        RORB  R3                    \n\
        RORB  R1                    \n\
        RORB  R3                    \n\
        ROLB  R1                    \n"
  );
} // _MIRROR_BYTE

void Laser2_MIRV (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
OUTLNG: MOV   R0, R5   // R0 left   \n\
        ADD   R2, R5   // R5 right  \n\
        MOV   R2, R4   // len       \n\
        INC   R4       // len + 1   \n\
        ASR   R4       // (len+1)/2 \n\
        MOV   R0, -(SP)  // scr adr \n\
        MOV   R3, -(SP)             \n\
OUTWRG: DEC   R5                    \n\
        MOVB  @R5, R1               \n\
        JSR   PC, __MIRROR_BYTE     \n\
        MOVB  @R0, R1               \n\
        MOVB  R3, @R0               \n\
        JSR   PC, __MIRROR_BYTE     \n\
        MOVB  R3, @R5               \n\
        INC   R0                    \n\
        SOB   R4, OUTWRG            \n\
        MOV   (SP)+, R3             \n\
        MOV   (SP)+, R0             \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLNG            \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_MIRV

/*------------------------------- Cut here --------------------------------*/
void Laser2_SL1V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2          // 2*len  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
        ADD   R2, R0    // scr+len  \n\
OUTLNA: MOV   R0, R1      // scr    \n\
        MOV   R2, R4      // len    \n\
        CLC                         \n\
OUTBYA: RORB  -(R1)                 \n\
        SOB   R4, OUTBYA            \n\
        INC   PC  // repeat twice   \n\
        BR    OUTLNA                \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLNA  // hgt    \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_SL1V

/*------------------------------- Cut here --------------------------------*/
void Laser2_SL4V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2                    \n\
        DEC   R2        // 2*len-1  \n\
        MOV   R4, -(SP)             \n\
OUTLNB: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
OUTBTB: MOVB  1(R1), (R1)+          \n\
        SOB   R4, OUTBTB            \n\
        CLRB  @R1                   \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLNB            \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_SL4V

/*------------------------------- Cut here --------------------------------*/
void Laser2_SL8V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        DEC   R2          // len-1  \n\
        MOV   R4, -(SP)             \n\
OUTLNC: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
OUTBTC: MOV   2(R1), (R1)+          \n\
        SOB   R4, OUTBTC            \n\
        CLR   @R1                   \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLNC            \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_SL8V

/*------------------------------- Cut here --------------------------------*/
void Laser2_SR1V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2          // 2*len  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
OUTLND: MOV   R0, R1      // scr    \n\
        MOV   R2, R4      // len    \n\
        CLC                         \n\
OUTBYD: ROLB  (R1)+                 \n\
        SOB   R4, OUTBYD            \n\
        INC   PC  // repeat twice   \n\
        BR    OUTLND                \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLND  // hgt    \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_SR1V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WL1V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2          // 2*len  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
        ADD   R2, R0    // scr+len  \n\
OUTLN3: MOV   R0, R1      // scr    \n\
        MOV   R2, R4      // len    \n\
        CLC                         \n\
OUTBY3: RORB  -(R1)                 \n\
        SOB   R4, OUTBY3            \n\
        BCC   NULPIX                \n\
        BIS   $0100000, -1(R0)      \n\
NULPIX: INC   PC  // repeat twice   \n\
        BR    OUTLN3                \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN3  // hgt    \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WL1V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WL4V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2                    \n\
        DEC   R2        // 2*len-1  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
OUTLN4: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
        MOVB  @R1, R5               \n\
OUTBT4: MOVB  1(R1), (R1)+          \n\
        SOB   R4, OUTBT4            \n\
        MOVB  R5, @R1               \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN4            \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WL4V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WL8V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        DEC   R2          // len-1  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
OUTLN5: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
        MOV   @R1, R5               \n\
OUTWR5: MOV   2(R1), (R1)+          \n\
        SOB   R4, OUTWR5            \n\
        MOV   R5, @R1               \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN5            \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WL8V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WR1V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2          // 2*len  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
OUTLN6: MOV   R0, R1      // scr    \n\
        MOV   R2, R4      // len    \n\
        CLC                         \n\
OUTBY6: ROLB  (R1)+                 \n\
        SOB   R4, OUTBY6            \n\
        ADC   @R0                   \n\
        INC   PC  // repeat twice   \n\
        BR    OUTLN6                \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN6  // hgt    \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WR1V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WR4V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        ASL   R2                    \n\
        DEC   R2        // 2*len-1  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
        ADD   R2, R0    // scr+len  \n\
OUTLN7: MOV   R0, R1      // scr    \n\
        MOV   R2, R4      // len    \n\
        MOVB  -1(R1), R5            \n\
OUTBT7: MOVB  -(R1), 1(R1)          \n\
        SOB   R4, OUTBT7            \n\
        MOVB  R5, @R1               \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN7            \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WR4V

/*------------------------------- Cut here --------------------------------*/
void Laser2_WR8V (int x, int y, int len, int hgt)
{
  asm("\
        JSR   PC, __Laser2_XYtoScr  \n\
        DEC   R2          // len-1  \n\
        MOV   R4, -(SP)             \n\
        MOV   R5, -(SP)             \n\
        ADD   R2, R0                \n\
        ADD   R2, R0  // scr+2*len  \n\
OUTLN8: MOV   R0, R1                \n\
        MOV   R2, R4                \n\
        MOV   @R1, R5               \n\
OUTWR8: MOV   -(R1), 2(R1)          \n\
        SOB   R4, OUTWR8            \n\
        MOV   R5, @R1               \n\
        ADD   $0100, R0             \n\
        SOB   R3, OUTLN8            \n\
        MOV   (SP)+, R5             \n\
        MOV   (SP)+, R4             \n"
      :::"r2", "r3"
  );
} // Laser2_WR8V
