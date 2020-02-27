#ifndef OSI_FUNCTIONS_H_
#define OSI_FUNCTIONS_H_

#include<iostream>
#include<sstream>
#include<string>
#include <coin-or/OsiClpSolverInterface.hpp>
#include <coin-or/CbcModel.hpp>
#include <coin-or/CoinError.hpp>
#include<nan.h>
using namespace v8;
using namespace Nan;

NAN_METHOD(solveLp);

#endif
