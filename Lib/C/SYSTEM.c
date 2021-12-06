int __mulhi3 (int a, int b)
{
  // Subroutine for multiplying 16-bit numbers
  // R0 - result, R1 - iteration counter,
  // R2 - multiplier, R3 - multiplicator
  // The algorithm of this program is built completely analogous
  // to the rule of multiplication "in column" for binary numbers
  // and probably does not need to explain.
  asm("\
        MOV  %0, R2                                       \n\
        MOV  %1, R3                                       \n\
        MOV  $020, R1  // A cycle of 16 iterations        \n\
        CLR  R0        // Clean the result accumulator    \n\
$1:     ASR  R3        // Next digit of the multiplicator \n\
        BCC  $2        // If it is not 0                  \n\
        ADD  R2, R0    //   Add a multiplier              \n\
$2:     ASL  R2        // One digit shift of multiplier   \n\
        SOB  R1, $1    // If not the end, into the loop   \n"
      ::"g"(a), "g"(b)
      :"r2", "r3", "r4"
  );
} // __mulhi3

/*------------------------------- Cut here --------------------------------*/
int __mulhi3_speed (int a, int b)
{
  asm("\
        MOV  %0, R1  \n\
        MOV  %1, R2  \n\
        CLR  R0      \n\
        CMP  R1, R2  \n\
        BHIS 1$      \n\
        XOR  R1, R2  \n\
        XOR  R2, R1  \n\
        XOR  R1, R2  \n\
1$:     TST  R2      \n\
        BEQ  0$      \n\
        CLC          \n\
        ROR  R2      \n\
        BCC  2$      \n\
        ADD  R1, R0  \n\
2$:     ASL  R1      \n\
        ADD  R1, R0  \n\
        ASR  R2      \n\
        BCS  2$      \n\
        BEQ  3$      \n\
        SUB  R1, R0  \n\
        BR   2$      \n\
3$:     SUB  R1, R0  \n\
0$:                  \n"
      ::"g"(a), "g"(b)
      :"r2"
  );
} // __mulhi3_speed

/*------------------------------- Cut here --------------------------------*/
int __divhi3 (int a, int b)
{
// R3-iteration counter, R0-result accumulator
  asm("\
        MOV  %0, R1  \n\
        MOV  %1, R2  \n\
        CLR  R3      \n\
        CLR  R0      \n\
1$:     INC  R3      \n\
        ASL  R2      \n\
        BCC  1$      \n\
2$:     ROR  R2      \n\
3$:     INC  R0      \n\
        SUB  R2, R1  \n\
        BCC  3$      \n\
        ADD  R2, R1  \n\
        DEC  R0      \n\
        ASL  R0      \n\
        SOB  R3, 2$  \n\
        ROR  R0      \n"
      ::"g"(a), "g"(b)
      :"r2", "r3"
  );
} // __divhi3

/*------------------------------- Cut here --------------------------------*/
int __modhi3 (int a, int b)
{
  asm("\
        MOV  %0, R0  \n\
        MOV  %1, R2  \n\
        CLR  R3      \n\
        CLR  R1      \n\
1$:     INC  R3      \n\
        ASL  R2      \n\
        BCC  1$      \n\
2$:     ROR  R2      \n\
3$:     INC  R1      \n\
        SUB  R2, R0  \n\
        BCC  3$      \n\
        ADD  R2, R0  \n\
        DEC  R1      \n\
        ASL  R1      \n\
        SOB  R3, 2$  \n"
      ::"g"(a), "g"(b)
      :"r2", "r3"
  );
} // __modhi3
