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

class OsiWorker : public AsyncProgressQueueWorker<char>
{
public:
    OsiWorker(std::string file_path, Callback* progress, Callback* end_callback);
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

    std::stringstream sout;
	OsiClpSolverInterface solver;
    std::string file_path;
    Nan::Callback* progress;
};

NAN_METHOD(solveLp);

#endif
