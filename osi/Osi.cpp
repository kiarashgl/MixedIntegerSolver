#include "Osi.h"

OsiSolver::OsiSolver(std::string file_path)
: file_path(file_path)
, optimal(false)
{
}

OsiWorker::OsiWorker(std::string file_path, Callback* progress, Callback* end_callback)
: AsyncProgressQueueWorker(end_callback)
, OsiSolver(file_path)
, progress(progress)
{
}

OsiWorker::~OsiWorker()
{
	delete progress;
}

void OsiWorker::Execute(const ExecutionProgress &progress)
{
	try
	{
		if (solver.readLp(file_path.c_str()) == -1)
			throw CoinError("Unable to Read Lp File", "readLp", "OsiClpSolverInterface");
		CbcModel model(solver);
		MessageHandler message_handler(progress);
		model.passInMessageHandler(&message_handler);
		Run(model);
	}
	catch (const CoinError &e)
	{
		optimal = false;
		progress.Send(reinterpret_cast<const char*>(e.message().c_str()), e.message().length());
	}
}

void OsiSolver::Execute()
{
	try
	{
		if (solver.readLp(file_path.c_str()) == -1)
			throw CoinError("Unable to Read Lp File", "readLp", "OsiClpSolverInterface");
		CbcModel model(solver);
		Run(model);
	}
	catch (const CoinError &e)
	{
		optimal = false;
		sout << e.message() << std::endl;
	}
}

void OsiSolver::Run(CbcModel& model)
{
	try
	{
		model.setKeepNamesPreproc(true);
		model.branchAndBound();

		if (model.isProvenOptimal())
		{
			optimal = true;
			response = "Found optimal solution!";
			objective_value = model.getObjValue();

			int n = model.getNumCols();
			const double *solution = model.getColSolution();

			for (int i = 0; i < n; ++i)
				answers.push_back({model.solver()->getColName(i), solution[i]});
		}
		else
		{
			response = "Didn't find optimal solution.";
			if (model.solver()->isProvenPrimalInfeasible())
				response += " Problem is proven to be infeasible.";
			if (model.solver()->isProvenDualInfeasible())
				response += " Problem is proven dual infeasible.";
			if (model.solver()->isPrimalObjectiveLimitReached())
				response += " Reached primal objective limit.";
			if (model.solver()->isDualObjectiveLimitReached())
				response += " Reached dual objective limit.";
			if (model.solver()->isIterationLimitReached())
				response += " Reached iteration limit.";
		}
	}
	catch (const CoinError &e)
	{
		optimal = false;
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

	createSout();
	v8::Local<v8::Value> argv[] = { Nan::New(sout.str()).ToLocalChecked(), Nan::New(getResultJson()).ToLocalChecked() };

	callback->Call(2, argv, async_resource);
}

std::string OsiSolver::getResultJson() noexcept
{
	std::string result = "{ ";
	result += "\"response\": \"" + response + "\"";
	if (optimal)
	{
		result += ", \"objective_value\": " + std::to_string(objective_value);
		result += ", \"values\": [";
		for (size_t i = 0; i < answers.size(); ++i)
		{
			if (i > 0)
				result += ", ";
			result += "{ \"name\": \"" + answers[i].first + "\", \"value\": " + std::to_string(answers[i].second) + "}"; 
		}
		result += "]";
	}
	result += "}";

	return result;
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

void OsiSolver::createSout() noexcept
{
	sout << response << std::endl;
	if (optimal)
	{
		sout << "Objective value is " << objective_value << std::endl;
		for (auto ans : answers)
			sout << ans.first << " = " << ans.second << std::endl;
	}
		
	sout.flush();
}

NAN_METHOD(solveWeb)
{
    Isolate* isolate = info.GetIsolate();
    const std::string file_path(*v8::String::Utf8Value(isolate, info[0].As<String>()));
	Callback* progress = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
	Callback* end_callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
	AsyncQueueWorker(new OsiWorker(std::move(file_path), progress, end_callback));
}

NAN_METHOD(solveJson)
{
    Isolate* isolate = info.GetIsolate();
    const std::string file_path(*v8::String::Utf8Value(isolate, info[0].As<String>()));
	OsiSolver worker(std::move(file_path));
	worker.Execute();

	std::string result_json = worker.getResultJson();
	info.GetReturnValue().Set(Nan::New(result_json.c_str()).ToLocalChecked());
}
