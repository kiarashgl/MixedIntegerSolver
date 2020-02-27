#include "Osi.h"

OsiWorker::OsiWorker(std::string file_path, Callback* progress, Callback* end_callback)
: AsyncProgressQueueWorker(end_callback)
, file_path(file_path)
, progress(progress)
{
}

OsiWorker::~OsiWorker()
{
	delete progress;
}

void OsiWorker::Execute(const ExecutionProgress &progress)
{
    MessageHandler message_handler(progress);
	try
	{
		solver.readLp(file_path.c_str());
		CbcModel model(solver);
		model.passInMessageHandler(&message_handler);
		model.setKeepNamesPreproc(true);
		model.branchAndBound();

		if (model.isProvenOptimal())
		{
			sout << "Found optimal solution!" << std::endl;
			sout << "Objective value is " << model.getObjValue() << std::endl;

			int n = model.getNumCols();
			const double *solution = model.getColSolution();

			for (int i = 0; i < n; ++i)
				sout << model.solver()->getColName(i) << " = " << solution[i] << std::endl;
		}
		else
		{
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
	catch (const CoinError &e)
	{
		sout << e.message() << std::endl;
	}
}

void OsiWorker::HandleProgressCallback(const char *data, size_t count)
{
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {
        Nan::New<String>(std::string(data)).ToLocalChecked()
    };
    progress->Call(1, argv, async_resource);

}

void OsiWorker::HandleOKCallback()
{
	Nan::HandleScope scope;

	v8::Local<v8::Value> argv[] = {Nan::New(sout.str()).ToLocalChecked()};
	callback->Call(1, argv, async_resource);
}

OsiWorker::MessageHandler::MessageHandler(const AsyncProgressQueueWorker<char>::ExecutionProgress& progress)
: progress(progress)
{
}

int OsiWorker::MessageHandler::print()
{
	this->progress.Send(reinterpret_cast<const char*>(messageBuffer()), strlen(messageBuffer()));
	// Used some delay to ensure that the message is delivered to Node.Js 
	usleep(100 * 1000);

	return CoinMessageHandler::print();
}

NAN_METHOD(solveLp)
{
    Isolate* isolate = info.GetIsolate();
    const std::string file_path(*v8::String::Utf8Value(isolate, info[0].As<String>()));
	Callback* progress = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
	Callback* end_callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
	AsyncQueueWorker(new OsiWorker(std::move(file_path), progress, end_callback));
}
