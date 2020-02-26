#include "Osi.h"

using namespace v8;
using namespace Nan;

NAN_METHOD(solveLp)
{
    Isolate * isolate = info.GetIsolate();
    const std::string file_path(*v8::String::Utf8Value(isolate, info[0].As<String>()));
    std::stringstream sout;
    OsiClpSolverInterface solver;

   try
   {
      solver.readLp(file_path.c_str());
      CbcModel model(solver);
      model.branchAndBound();

      if (model.isProvenOptimal() ) {
         sout << "Found optimal solution!" << std::endl; 
         sout << "Objective value is " << model.getObjValue() << std::endl;

         int n = model.getNumCols();
         const double* solution = model.getColSolution();

         for( int i = 0; i < n; ++i )
            sout << solver.getColName(i) << " = " << solution[i] << std::endl;
      } else {
         sout << "Didn't find optimal solution." << std::endl;
         if (model.solver()->isProvenPrimalInfeasible())
            sout << "Problem is proven to be infeasible." << std::endl;
         if (model.solver()->isProvenDualInfeasible())
            sout << "Problem is proven dual infeasible." << std::endl;
         if (model.solver()->isPrimalObjectiveLimitReached())
            sout << "Reached primal objective limit." << std::endl;
         if (model.solver()->isDualObjectiveLimitReached())
            sout << "Reached dual objective limit." << std::endl;
         if (model.solver()->isIterationLimitReached())
            sout << "Reached iteration limit." << std::endl;
      }
   }
   catch(CoinError e)
   {
      sout << e.message() << std::endl;
   }
   info.GetReturnValue().Set(Nan::New(sout.str()).ToLocalChecked());

}