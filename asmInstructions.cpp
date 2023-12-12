#pragma once
#include <iostream>
using namespace std;
enum AsmDataType
{
  WORD,
  ASCIIZ,
  SPACE
};

const char* asmDataToString(AsmDataType t) {
  switch (t){
  case WORD:
    return ".word";
  case ASCIIZ:
    return ".asciiz";
  case SPACE:
    return ".space";
  default:
    return "UNKNOWN DATA TYPE";
  }
}

enum AsmRegisters
{
  ZERO,
  V0,
  V1,
  A0,
  A1,
  A2,
  A3,
  T0,
  T1,
  T2,
  T3,
  T4,
  T5,
  T6,
  T7,
  T8,
  T9,
  S0,
  S1,
  S2,
  S3,
  S4,
  S5,
  S6,
  S7
};

string asmRegToString(AsmRegisters t) {
  string result = "$";
  switch (t){
  case ZERO:
    result += "zero";
    break;
  case V0:
    result += "v0";
    break;
  case V1:
    result += "v1";
    break;
  case A0:
    result += "a0";
    break;
  case A1:
    result += "a1";
    break;
  case A2:
    result += "a2";
    break;
  case A3:
    result += "a3";
    break;
  case T0:
    result += "t0";
    break;
  case T1:
    result += "t1";
    break;
  case T2:
    result += "t2";
    break;
  case T3:
    result += "t3";
    break;
  case T4:
    result += "t4";
    break;
  case T5:
    result += "t5";
    break;
  case T6:
    result += "t6";
    break;
  case T7:
    result += "t7";
    break;
  case T8:
    result += "t8";
    break;
  case T9:
    result += "t9";
    break;
  case S0:
    result += "s0";
    break;
  case S1:
    result += "s1";
    break;
  case S2:
    result += "s2";
    break;
  case S3:
    result += "s3";
    break;
  case S4:
    result += "s4";
    break;
  case S5:
    result += "s5";
    break;
  case S6:
    result += "s6";
    break;
  case S7:
    result += "s7";
    break;
  default:
    break;
  }

  return result;
}
