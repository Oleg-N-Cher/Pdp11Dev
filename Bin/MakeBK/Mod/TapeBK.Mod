MODULE TapeBK;	(** portable *)

IMPORT
  SYSTEM, Files;

TYPE
  BYTE = SYSTEM.BYTE;
  WORD = INT32; (* Alias for INTEGER type to enhance the visibility. *)
  
  (* TAP format: *)
  TapeFile* = RECORD [notag]
    file: Files.FileToWrite;
    error-: BOOLEAN; (** Was there an error during the work? *)
    errcode-: WORD;  (** A code of the first occurred error. *)
    checksum: WORD;
  END;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) ReCreate* (tapeName: ARRAY OF CHAR);
BEGIN
  tap.file.OpenToWrite(tapeName);
  tap.error := tap.file.error; (* Выносим обработку ошибок на уровень выше. *)
END ReCreate;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) SaveByte (byte: BYTE);
BEGIN
  IF ~tap.error THEN
    tap.file.WriteByte(byte); tap.error := tap.file.error;
    INC(tap.checksum, byte);
    tap.checksum := (ORD(tap.checksum > 0FFFFH) + tap.checksum) MOD 10000H;
  END;
END SaveByte;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) SaveWord (word: WORD);
BEGIN
  ASSERT((word >= 0) & (word <= 0FFFFH)); (* word IN {0..65535} *)
  IF ~tap.error THEN
    tap.SaveByte(SHORT(SHORT(word MOD 100H))); tap.error := tap.file.error;
    IF ~tap.error THEN
      tap.SaveByte(SHORT(SHORT(word DIV 100H))); tap.error := tap.file.error;
    END;
  END;
END SaveWord;

(*----------------------------------------------------------------------------*)

(* Контрольная сумма считается так (R5 - адрес, R4 - длина):
	CLR R0
1:	CLR R2
	BISB (R5)+,R2
	ADD R2,R0
	ADC R0
	SOB R4,1
	RET
*)
PROCEDURE (VAR tap: TapeFile) SaveCheckSum;
BEGIN
  tap.SaveWord(tap.checksum);
END SaveCheckSum;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) SaveStr (str: ARRAY OF CHAR);
VAR
  i: INTEGER;
BEGIN
  i := 0;
  WHILE (str[i] # 0X) & (i < LEN(str)) DO 
    tap.SaveByte(SHORT(ORD(str[i]))); INC(i);
  END;
END SaveStr;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) SaveDataBlock (
  dataLength: INTEGER; VAR data: ARRAY OF BYTE);
VAR
  i: INTEGER;
BEGIN
  FOR i := 0 TO dataLength - 1 DO tap.SaveByte(data[i]) END;
END SaveDataBlock;

(*----------------------------------------------------------------------------*)
(* Формат BIN-файла:
      2 байта - адрес загрузки программы,
      2 байта - длина файла,
      затем идёт массив
      2 байта - контрольная сумма,
        которая, в принципе, нигде не используется
        и может служить для контроля целостности файла.
*)
PROCEDURE (VAR tap: TapeFile) SaveCode* (
  name: ARRAY OF CHAR; startAddr, dataLength: INTEGER; VAR data: ARRAY OF BYTE);
BEGIN
  tap.SaveWord(startAddr);
  tap.SaveWord(dataLength);
  tap.checksum := 0;
  tap.SaveDataBlock(dataLength, @data);
  tap.SaveCheckSum;
END SaveCode;

(*----------------------------------------------------------------------------*)
PROCEDURE (VAR tap: TapeFile) Finalize* ;
BEGIN
  tap.file.Close;
END Finalize;

END TapeBK.
