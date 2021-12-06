MODULE MakeBK; (** portable *)

IMPORT
  SYSTEM, IO := Console, Cmd := CmdLine, Files, TapeBK;

CONST
  CodeMaxSize = 10000H;

VAR
  name: Cmd.String; (* A tape/code name. *)
  code: ARRAY CodeMaxSize OF SYSTEM.BYTE; codeLen: INT32;
  codeStartAddr: INT32;
  codeFile: Files.FileToRead;
  tap: TapeBK.TapeFile;

PROCEDURE WriteStrLn (str: ARRAY OF CHAR);
BEGIN
  IO.WriteStr(str); IO.WriteLn;
END WriteStrLn;

PROCEDURE CreateBIN;
  VAR binName, tapName: Cmd.String;
BEGIN
  (* LOAD CODE: *)
  binName := name + ".bin";
  codeFile.OpenToRead(binName);
  IF codeFile.error OR codeFile.end THEN
    IO.WriteStr("Can't open file."); IO.WriteLn; HALT(1)
  END;
  codeLen := 0;
  WHILE ~ (codeFile.error OR codeFile.end) DO
    code[codeLen] := codeFile.ReadByte(); INC(codeLen);
  END;
  IF codeFile.error THEN
    codeFile.Close;
    IO.WriteStr("Can't read file."); IO.WriteLn; HALT(1)
  END;
  codeFile.Close;

  (* SAVE CODE: *)
  tapName := name + ".bin";
  tap.ReCreate(tapName);
  IF tap.error THEN
    IO.WriteStr("Can't create tape file"); IO.WriteLn; HALT(1)
  END;
  tap.SaveCode(name, codeStartAddr, codeLen, @code);
  tap.Finalize;
  IF tap.error THEN
    IO.WriteStr("Can't save tape file"); IO.WriteLn; HALT(1)
  END;
END CreateBIN;

BEGIN (*$MAIN*)
  WriteStrLn("MakeBK v1.0: convert a binary file to a BK emulator format");
  WriteStrLn("Copyright (C) 2021 Oleg N. Cher, VEDAsoft Oberon Club" );
  WriteStrLn("  https://github.com/Oleg-N-Cher/Pdp11Dev");
  IO.WriteLn;
  CASE Cmd.ParamCount OF
  | 1: (* One parameter as input binary file name and output tape name: *)
    Cmd.GetParam(1, @name); codeStartAddr := 200H; CreateBIN;
  | 2: (* Two parameters as input binary file name and code address: *)
    Cmd.GetParam(1, @name); Cmd.GetParamInt(2, @codeStartAddr); CreateBIN;
  ELSE (* ShowUsage: *)
    WriteStrLn("Usage: makebk filename [starting address]");
    WriteStrLn("  Example: makebk mygame 512"); HALT(1)
  END;
END MakeBK.
