#pragma once

enum AsmDataType
{
  WORD,
  ASCIIZ,
};

const char* asmDataToString(AsmDataType t) {
  switch (t){
  case WORD:
    return ".word";
  case ASCIIZ:
    return ".asciiz";
  default:
    return "UNKNOWN DATA TYPE";
  }
}
