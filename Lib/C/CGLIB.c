extern int xlast, ylast, lcol;

/*================================ Header =================================*/
int xlast, ylast;
/*------------------------------- Cut here --------------------------------*/
int lcol;
/*------------------------------- Cut here --------------------------------*/
void InitGraph (void)
{
  asm("  .GLOBL  TTYOUT              \n"
      "  BIS     $010000, @$044      \n"  // Disable Ctrl-C
      "  MOV     $INIT$, R0          \n"
"TTYOUT: TSTB    @$0177564           \n"
      "  BPL     TTYOUT              \n"
      "  MOVB    (R0)+, @$0177566    \n"
      "  BNE     TTYOUT              \n"
      "  MOV     $0177777, R0        \n"  // Pause to let PPU finish the previous commands
"PAUSE0: NOP                         \n"
      "  SOB     R0, PAUSE0          \n"
      "  RTS     PC                  \n"
"INIT$:  .BYTE   033, 0246, 062      \n"  // Screen mode 40x24
      "  .BYTE   033, 045, 041, 061  \n"  // Text mode 1
      "  .BYTE   033, 0133, 060, 073, 060, 0162  \n"
      "  .BYTE   033, 045, 041, 060  \n"  // Graph
      "  .BYTE	 033, 0240, 061	     \n"  // Цвет символа
      "  .BYTE	 033, 0241, 060	     \n"  // Цвет знакоместа 0
      "  .BYTE	 033, 0242, 060	     \n"  // Цвет фона 0
      "  .BYTE   014                 \n"
      "  .BYTE   0                   \n"
  );
} // InitGraph

/*------------------------------- Cut here --------------------------------*/

void ClearDevice (void)
{
  asm("  MOV     R2, -(SP)     \n"
      "  MOV     $0176640, R0  \n"
      "  MOV     $0176642, R1  \n"
      "  MOV     $0100000, R2  \n"
      "  MOV     R2, @R0       \n"
"1$:     CLR     @R1           \n"
      "  INC     @R0           \n"
      "  SOB     R2, 1$        \n"
      "  MOV     (SP)+, R2     \n"
  );
} // ClearDevice

/*------------------------------- Cut here --------------------------------*/

void PALET (void)
{
  asm("  .GLOBL  NEWPAL            \n"
      "  MOV     R0, NEWPAL        \n"
      "  MOV     R1, NEWPAL+2      \n"
      "  MOV     $BUF0, R0         \n"
      "  MOV     $5, R1            \n"
      "  MTPS    $0200             \n"
      "  BR      2$                \n"
"1$:     MOVB    (R0)+, @$0176676  \n"
"2$:     TSTB    @$0176674         \n"
      "  BPL     2$                \n"
      "  SOB     R1, 1$            \n"
      "  MTPS    $0                \n"
      "  RTS     PC                \n"
"BLK:    .WORD   020*0400, 032, 02470, NEWPAL, 2         \n"
"BUF0:   .WORD   BLK, -1                                 \n"
"NEWPAL: .WORD   0b1110111011001100, 0b1010101010001000  \n"
  );
} // PALET

/*------------------------------- Cut here --------------------------------*/

void Palette (int n)
{
  asm("  MOV     $0146000, R0      ; #CC00  \n"
      "  MOV     $0167252, R1      ; #EEAA  \n"
      "  BICB    $0177774, 2(SP)            \n"
      "  BEQ     PALET$                     \n"
      "  MOV     $0135631, R0      ; #BB99  \n"
      "  MOV     $0177735, R1      ; #FFDD  \n"
      "  DEC     2(SP)                      \n"
      "  BEQ     PALET$                     \n"
      "  MOV     $042000, R0       ; #4400  \n"
      "  MOV     $021146, R1       ; #2266  \n"
      "  DEC     2(SP)                      \n"
      "  BEQ     PALET$                     \n"
      "  MOV     $031421, R0       ; #3311  \n"
      "  MOV     $073525, R1       ; #7755  \n"
"PALET$:                                    \n"
  );
  PALET();
} // Palette

/*------------------------------- Cut here --------------------------------*/

void SetPalette (int c0, int c1, int c2, int c3)
{
  asm("  BICB    $0177760, 8(SP)  \n"
      "  BICB    $0177760, 6(SP)  \n"
      "  MOVB    6(SP), R0        \n"
      "  ASH     $4, R0           \n"
      "  ADD     6(SP), R0        \n"
      "  ASH     $4, R0           \n"
      "  ADD     8(SP), R0        \n"
      "  ASH     $4, R0           \n"
      "  ADD     8(SP), R0        \n"
      "  BICB    $0177760, 4(SP)  \n"
      "  BICB    $0177760, 2(SP)  \n"
      "  MOVB    2(SP), R1        \n"
      "  ASH     $4, R1           \n"
      "  ADD     2(SP), R1        \n"
      "  ASH     $4, R1           \n"
      "  ADD     4(SP), R1        \n"
      "  ASH     $4, R1           \n"
      "  ADD     4(SP), R1        \n"
  );
  PALET();
} // SetPalette

/*------------------------------- Cut here --------------------------------*/

void PutPixel (int x, int y, int color)
{
  asm( "  MOV    2(SP), R1      \n"
       "  MOV    R1, _xlast     \n"
       "  BIC    $0177770, R1   \n"
       "  MOV    $1, R0         \n"
       "  ASH    R1, R0         \n"
       "  MOV    4(SP), R1      \n"
       "  MOV    R1, _ylast     \n"
       "  MUL    $0120, R1      \n"
       "  BIS    $0100000, R1   \n"
       "  MOV    R1, @$0176640  \n"
       "  MOV    2(SP), R1      \n"
       "  ASH    $-3, R1        \n"
       "  ADD    R1, @$0176640  \n"
       "  BICB   R0, @$0176642  \n"
       "  BICB   R0, @$0176643  \n"
       "  BIT    $1, 6(SP)      \n"
       "  BEQ    1$             \n"
       "  BISB   R0, @$0176642  \n"
"1$:      BIT    $2, 6(SP)      \n"
       "  BEQ    2$             \n"
       "  BISB   R0, @$0176643  \n"
"2$:                            \n"
  );
}

/*------------------------------- Cut here --------------------------------*/

void Line (int x1, int y1, int x2, int y2)
{
  asm( "  .GLOBL _PutPixel         \n"
       "  MOV    R2, -(SP)         \n"
       "  MOV    R3, -(SP)         \n"
       "  MOV    R4, -(SP)         \n"
       "  MOV    014(SP), _xlast   \n"
       "  MOV    016(SP), _ylast   \n"
       "  MOV    010(SP), B.4      \n"
       "  MOV    012(SP), B.6      \n"
       "  JSR    PC, B.354         \n"
       "  CMP    010(SP), 014(SP)  \n"
       "  BCC    B.72              \n"
       "  MOV    $1, R2            \n"
       "  BR     B.76              \n"
"B.72:    MOV    $0177777, R2      \n"
"B.76:    CMP    012(SP), 016(SP)  \n"
       "  BCC    B.114             \n"
       "  MOV    $1, R3            \n"
       "  BR     B.120             \n"
"B.114:   MOV    $0177777, R3      \n"
"B.120:   MOV    010(SP), B.0      \n"
       "  SUB    014(SP), B.0      \n"
       "  BPL    B.142             \n"
       "  NEG    B.0               \n"
"B.142:   MOV    012(SP), B.2      \n"
       "  SUB    016(SP), B.2      \n"
       "  BPL    B.164             \n"
       "  NEG    B.2               \n"
"B.164:   CMP    B.0, B.2          \n"
       "  BLOS   B.260             \n"
       "  MOV    B.0, R4           \n"
       "  ASR    R4                \n"
       "  MOV    $1, B.10          \n"
"B.210:   CMP    B.10, B.0         \n"
       "  BHI    B.344             \n"
       "  ADD    R2, B.4           \n"
       "  ADD    B.2, R4           \n"
       "  CMP    R4, B.0           \n"
       "  BCS    B.246             \n"
       "  SUB    B.0, R4           \n"
       "  ADD    R3, B.6           \n"
"B.246:   JSR    PC, B.354         \n"
       "  INC    B.10              \n"
       "  BR     B.210             \n"
"B.260:   MOV    B.2, R4           \n"
       "  ASR    R4                \n"
       "  MOV    $1, B.10          \n"
"B.274:   CMP    B.10, B.2         \n"
       "  BHI    B.344             \n"
       "  ADD    R3, B.6           \n"
       "  ADD    B.0, R4           \n"
       "  CMP    R4, B.2           \n"
       "  BCS    B.332             \n"
       "  SUB    B.2, R4           \n"
       "  ADD    R2, B.4           \n"
"B.332:   JSR    PC, B.354         \n"
       "  INC    B.10              \n"
       "  BR     B.274             \n"
"B.344:   MOV    (SP)+, R4         \n"
       "  MOV    (SP)+, R3         \n"
       "  MOV    (SP)+, R2         \n"
       "  RTS    PC                \n"
"B.354:   MOV    _lcol, -(SP)      \n"
       "  MOV    B.6, -(SP)        \n"
       "  MOV    B.4, -(SP)        \n"
       "  JSR    PC, _PutPixel     \n"
       "  ADD    $6, SP            \n"
       "  RTS    PC                \n"
"B.0:     .WORD  0                 \n"
"B.2:     .WORD  0                 \n"
"B.4:     .WORD  0                 \n"
"B.6:     .WORD  0                 \n"
"B.10:    .WORD  0                 \n"
  );
} // Line

/*------------------------------- Cut here --------------------------------*/

void SetLineColor (int color)
{
  asm( "  MOV    2(SP), _lcol      \n"
       "  BIC    $0177774, _lcol   \n"
  );
} // SetLineColor
