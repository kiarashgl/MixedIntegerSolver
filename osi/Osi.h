#ifndef OSI_FUNCTIONS_H_
#define OSI_FUNCTIONS_H_

#include<iostream>
#include<sstream>
#include<string>
#include <unistd.h>

#include <coin-or/OsiClpSolverInterface.hpp>
#include <coin-or/CbcModel.hpp>
#include <coin-or/CoinError.hpp>
#include <nan.h>

using namespace v8;
using namespace Nan;

class OsiSolver
{
public:
    OsiSolver(std::string file_path);

    void Execute();
    void Run(CbcModel& model);
    std::string getResultJson() noexcept;
    void createSout() noexcept;

protected:
    std::stringstream sout;
	OsiClpSolverInterface solver;
    const std::string file_path;
    bool optimal;
    std::string response;
    double objective_value;
    std::vector<std::pair<std::string, double>> answers;
};

class OsiWorker : public AsyncProgressQueueWorker<char>, public OsiSolver
{
public:
    OsiWorker(std::string file_path, Callback* progress, Callback* end_callback);
    OsiWorker() = delete;
    ~OsiWorker();

    void Execute(const ExecutionProgress& progress);
    
    void HandleProgressCallback(const char* data, size_t count);
    void HandleOKCallback() override;

private:
    class MessageHandler: public CoinMessageHandler
    {
    public:
        MessageHandler(const ExecutionProgress& progress);
            
        virtual int print();

    private:
        const AsyncProgressQueueWorker<char>::ExecutionProgress& progress;
    };

    Nan::Callback* progress;
};

NAN_METHOD(solveWeb);
NAN_METHOD(solveJson);

#endif
