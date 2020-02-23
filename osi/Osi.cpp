#include "Osi.h"

using namespace v8;
using namespace Nan;

NAN_METHOD(solveLp)
{
    Isolate * isolate = info.GetIsolate();
    const std::string file_path(*v8::String::Utf8Value(isolate, info[0].As<String>()));
    std::stringstream sout;
    OsiSolverInterface *solver = new OsiClpSolverInterface();

   try
   {
      solver->readLp(file_path.c_str());
   
      solver->initialSolve();

      if ( solver->isProvenOptimal() ) { 
         sout << "Found optimal solution!" << std::endl; 
         sout << "Objective value is " << solver->getObjValue() << std::endl;

         int n = solver->getNumCols();
         const double* solution = solver->getColSolution();

         for( int i = 0; i < n; ++i )
            sout << solver->getColName(i) << " = " << solution[i] << std::endl;
      } else {
         sout << "Didn't find optimal solution." << std::endl;
         if (solver->isProvenPrimalInfeasible())
            sout << "Problem is proven to be infeasible." << std::endl;
         if (solver->isProvenDualInfeasible())
            sout << "Problem is proven dual infeasible." << std::endl;
         if (solver->isPrimalObjectiveLimitReached())
            sout << "Reached primal objective limit." << std::endl;
         if (solver->isDualObjectiveLimitReached())
            sout << "Reached dual objective limit." << std::endl;
         if (solver->isIterationLimitReached())
            sout << "Reached iteration limit." << std::endl;
      }
   }
   catch(CoinError e)
   {
      sout << e.message() << std::endl;
   }
   info.GetReturnValue().Set(Nan::New(sout.str()).ToLocalChecked());

}