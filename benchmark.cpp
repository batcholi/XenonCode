#define XENONCODE_IMPLEMENTATION
#include "XenonCode.hpp"
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Benchmark result structure
struct BenchmarkResult {
	string name;
	vector<double> samples_ms;
	uint64_t iterations;

	double min() const {
		return *min_element(samples_ms.begin(), samples_ms.end());
	}
	double max() const {
		return *max_element(samples_ms.begin(), samples_ms.end());
	}
	double avg() const {
		return accumulate(samples_ms.begin(), samples_ms.end(), 0.0) / samples_ms.size();
	}
	double stddev() const {
		double mean = avg();
		double sq_sum = 0;
		for (double v : samples_ms) sq_sum += (v - mean) * (v - mean);
		return sqrt(sq_sum / samples_ms.size());
	}
	double per_iteration_us() const {
		return (avg() * 1000.0) / iterations;
	}
};

vector<BenchmarkResult> results;

// Timer helper
class Timer {
	high_resolution_clock::time_point start;
public:
	Timer() : start(high_resolution_clock::now()) {}
	double elapsed_ms() const {
		return duration<double, milli>(high_resolution_clock::now() - start).count();
	}
};

// Device function call counter for verification
static uint64_t deviceCallCount = 0;

void Init() {
	// Define numeric global constants
	XenonCode::DeclareGlobalConstant("pi", 3.141592653589793238462643);
	XenonCode::DeclareGlobalConstant("2pi", 2 * 3.141592653589793238462643);
	XenonCode::DeclareGlobalConstant("number_one", 1);
	XenonCode::DeclareGlobalConstant("number_two", 2);
	XenonCode::DeclareGlobalConstant("number_three", 3);

	// Define text global constants
	XenonCode::DeclareGlobalConstant("test_str1", "This is test string 1");
	XenonCode::DeclareGlobalConstant("test_str2", "This is test string 2");

	// Define callable entrypoints
	XenonCode::DeclareEntryPoint("shutdown");

	// Object type for testing
	auto positionType = XenonCode::DeclareObjectType("position", {
		{"x:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 1.0;
		}},
		{"y:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 2.0;
		}},
		{"z:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 3.0;
		}},
		{"xyz():text", [](XenonCode::Computer*, const XenonCode::Var&, const vector<XenonCode::Var>&) -> XenonCode::Var {
			return XenonCode::Var(".x{1}.y{2}.z{3}");
		}},
		{"normalize()", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return {};
		}},
	});

	// Device functions for benchmarking
	XenonCode::DeclareDeviceFunction("delta:number", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		return 0.016; // Simulated frame delta
	});

	XenonCode::DeclareDeviceFunction("print", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		// No-op for benchmarking (avoid I/O overhead)
		return {};
	});

	XenonCode::DeclareDeviceFunction("position:position", [=](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		return {positionType, 0};
	});

	// Additional device functions for stress testing
	XenonCode::DeclareDeviceFunction("benchmark_noop", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		return {};
	});

	XenonCode::DeclareDeviceFunction("benchmark_return_num:number", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		return 42.0;
	});

	XenonCode::DeclareDeviceFunction("benchmark_return_text:text", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		return XenonCode::Var("benchmark");
	});

	XenonCode::DeclareDeviceFunction("benchmark_with_args:number(a:number, b:number)", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		deviceCallCount++;
		if (args.size() >= 2) {
			return double(args[0]) + double(args[1]);
		}
		return 0.0;
	});

	// Silent output function
	XenonCode::SetOutputFunction([](XenonCode::Computer*, uint32_t ioIndex, const vector<XenonCode::Var>& args){
		// No-op for benchmarking
	});
}

// Run a XenonCode program and return execution time
double runProgram(const string& directory, bool compile = true) {
	if (compile) {
		auto mainFile = XenonCode::GetParsedFile(directory, "main.xc");
		if (!XenonCode::Computer::CompileAssembly(directory, mainFile.lines, false)) {
			cerr << "Failed to compile " << directory << endl;
			return -1;
		}
	}

	XenonCode::Computer computer;
	computer.capability.ram = 65536;

	if (!computer.LoadProgram(directory)) {
		cerr << "Failed to load " << directory << endl;
		return -1;
	}

	Timer timer;
	try {
		computer.RunInit();
	} catch (XenonCode::RuntimeError& e) {
		cerr << "Runtime error: " << e.what() << endl;
		return -1;
	}
	return timer.elapsed_ms();
}

// Benchmark a program with multiple samples
BenchmarkResult benchmarkProgram(const string& name, const string& directory, int samples = 10, int warmup = 2) {
	BenchmarkResult result;
	result.name = name;
	result.iterations = 1;

	// Compile once
	auto mainFile = XenonCode::GetParsedFile(directory, "main.xc");
	if (!XenonCode::Computer::CompileAssembly(directory, mainFile.lines, false)) {
		cerr << "Failed to compile " << directory << endl;
		return result;
	}

	// Warmup runs
	for (int i = 0; i < warmup; i++) {
		runProgram(directory, false);
	}

	// Benchmark runs
	for (int i = 0; i < samples; i++) {
		double time = runProgram(directory, false);
		if (time >= 0) {
			result.samples_ms.push_back(time);
		}
	}

	return result;
}

void printResult(const BenchmarkResult& r) {
	cout << left << setw(35) << r.name
		 << right << setw(12) << fixed << setprecision(3) << r.avg() << " ms"
		 << setw(12) << r.min() << " ms (min)"
		 << setw(12) << r.max() << " ms (max)"
		 << setw(12) << r.stddev() << " ms (std)"
		 << endl;
}

void printResultsCSV() {
	cout << "\n--- CSV Output ---\n";
	cout << "name,avg_ms,min_ms,max_ms,stddev_ms,iterations,per_iter_us\n";
	for (const auto& r : results) {
		cout << r.name << ","
			 << r.avg() << ","
			 << r.min() << ","
			 << r.max() << ","
			 << r.stddev() << ","
			 << r.iterations << ","
			 << r.per_iteration_us()
			 << "\n";
	}
}

void printSummary() {
	cout << "\n=== Benchmark Summary ===\n\n";
	cout << left << setw(35) << "Benchmark"
		 << right << setw(15) << "Avg"
		 << setw(15) << "Min"
		 << setw(15) << "Max"
		 << setw(15) << "Stddev"
		 << endl;
	cout << string(95, '-') << endl;

	for (const auto& r : results) {
		printResult(r);
	}
}

int main(int argc, char** argv) {
	Init();

	int samples = 10;
	int warmup = 3;
	bool csvOutput = false;
	string specificBenchmark = "";

	// Parse arguments
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-samples" && i + 1 < argc) {
			samples = atoi(argv[++i]);
		} else if (arg == "-warmup" && i + 1 < argc) {
			warmup = atoi(argv[++i]);
		} else if (arg == "-csv") {
			csvOutput = true;
		} else if (arg == "-benchmark" && i + 1 < argc) {
			specificBenchmark = argv[++i];
		} else if (arg == "-help") {
			cout << "XenonCode Benchmark Tool\n\n";
			cout << "Usage: benchmark [options]\n\n";
			cout << "Options:\n";
			cout << "  -samples N     Number of samples per benchmark (default: 10)\n";
			cout << "  -warmup N      Number of warmup runs (default: 3)\n";
			cout << "  -csv           Output results in CSV format\n";
			cout << "  -benchmark X   Run only benchmark named X\n";
			cout << "  -help          Show this help\n";
			return 0;
		}
	}

	cout << "XenonCode Benchmark Tool\n";
	cout << "========================\n\n";
	cout << "Configuration: " << samples << " samples, " << warmup << " warmup runs\n\n";

	// Benchmark 1: Unit tests (comprehensive)
	if (specificBenchmark.empty() || specificBenchmark == "test") {
		cout << "Running: Unit Test Suite...\n";
		deviceCallCount = 0;
		auto r = benchmarkProgram("Unit Test Suite", "test", samples, warmup);
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 2: Existing benchmark program
	if (specificBenchmark.empty() || specificBenchmark == "benchmark") {
		cout << "Running: Benchmark Program (25k iterations)...\n";
		deviceCallCount = 0;
		auto r = benchmarkProgram("Benchmark Program", "projects/benchmark", samples, warmup);
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 3: Device function stress test
	// Create a temporary XenonCode program for device call benchmarking
	if (specificBenchmark.empty() || specificBenchmark == "device") {
		cout << "Running: Device Function Stress Test...\n";

		// Create temp directory and program
		string tempDir = "projects/bench_device";
		system(("mkdir -p " + tempDir).c_str());

		// Write a device call stress test program
		ofstream f(tempDir + "/main.xc");
		f << R"(
; Device function stress test - 100k device calls
init
	var $result = 0
	repeat 100000 ($i)
		var $x = benchmark_return_num
		$result += $x
	output.0 ($result)
)";
		f.close();

		deviceCallCount = 0;
		auto r = benchmarkProgram("Device Calls (100k)", tempDir, samples, warmup);
		r.iterations = 100000;
		results.push_back(r);
		printResult(r);
		cout << "  Device calls executed: " << deviceCallCount / samples << " per run\n";
	}

	// Benchmark 4: Device function with arguments
	if (specificBenchmark.empty() || specificBenchmark == "device_args") {
		cout << "Running: Device Function with Args...\n";

		string tempDir = "projects/bench_device_args";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Device function with arguments stress test
init
	var $result = 0
	repeat 100000 ($i)
		var $x = benchmark_with_args(1.5, 2.5)
		$result += $x
	output.0 ($result)
)";
		f.close();

		deviceCallCount = 0;
		auto r = benchmarkProgram("Device Calls with Args (100k)", tempDir, samples, warmup);
		r.iterations = 100000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 5: Math operations
	if (specificBenchmark.empty() || specificBenchmark == "math") {
		cout << "Running: Math Operations...\n";

		string tempDir = "projects/bench_math";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Math operations stress test
init
	var $result = 0
	repeat 100000 ($i)
		var $x = $i * 3.14159
		var $y = $x / 2.71828
		var $z = $y + $x - 1.0
		$result = $z * sin($x) + cos($y)
	output.0 ($result)
)";
		f.close();

		auto r = benchmarkProgram("Math Operations (100k)", tempDir, samples, warmup);
		r.iterations = 100000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 6: Array operations
	if (specificBenchmark.empty() || specificBenchmark == "array") {
		cout << "Running: Array Operations...\n";

		string tempDir = "projects/bench_array";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Array operations stress test
init
	array $arr:number
	repeat 10000 ($i)
		$arr.append($i)
	var $sum = $arr.sum
	var $avg = $arr.avg
	var $max = $arr.max
	var $min = $arr.min
	output.0 ($sum)
	output.1 ($avg)
)";
		f.close();

		auto r = benchmarkProgram("Array Operations (10k append)", tempDir, samples, warmup);
		r.iterations = 10000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 7: Text operations
	if (specificBenchmark.empty() || specificBenchmark == "text") {
		cout << "Running: Text Operations...\n";

		string tempDir = "projects/bench_text";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Text operations stress test
init
	var $text = ""
	repeat 1000 ($i)
		$text = text($i, "000000")
		var $upper = upper($text)
		var $lower = lower($upper)
		var $len = $lower.size
	output.0 ($text)
)";
		f.close();

		auto r = benchmarkProgram("Text Operations (1k)", tempDir, samples, warmup);
		r.iterations = 1000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 8: Object member access
	if (specificBenchmark.empty() || specificBenchmark == "object") {
		cout << "Running: Object Member Access...\n";

		string tempDir = "projects/bench_object";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Object member access stress test
init
	var $result = 0
	repeat 50000 ($i)
		var $pos = position()
		var $x = $pos.x
		var $y = $pos.y
		var $z = $pos.z
		$result = $x + $y + $z
	output.0 ($result)
)";
		f.close();

		deviceCallCount = 0;
		auto r = benchmarkProgram("Object Access (50k)", tempDir, samples, warmup);
		r.iterations = 50000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 9: Key-Value object access
	if (specificBenchmark.empty() || specificBenchmark == "keyvalue") {
		cout << "Running: Key-Value Object Access..." << endl;

		string tempDir = "projects/bench_keyvalue";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Key-value object access stress test
init
	var $obj = ".name{test}.value{42}.x{1.5}.y{2.5}.z{3.5}"
	var $result = 0
	repeat 10000 ($i)
		var $name = $obj.name
		var $val = $obj.value
		var $x = $obj.x
		var $y = $obj.y
		var $z = $obj.z
		$result += 1
	output.0 ($result)
)";
		f.close();

		auto r = benchmarkProgram("Key-Value Access (10k)", tempDir, samples, warmup);
		r.iterations = 10000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 10: Foreach loop
	if (specificBenchmark.empty() || specificBenchmark == "foreach") {
		cout << "Running: Foreach Loop..." << endl;

		string tempDir = "projects/bench_foreach";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Foreach loop stress test
init
	array $arr:number
	repeat 10000 ($i)
		$arr.append($i * 0.5)
	var $total = 0
	foreach $arr ($idx, $value)
		$total += $value * 2
	output.0 ($total)
)";
		f.close();

		auto r = benchmarkProgram("Foreach Loop (10k)", tempDir, samples, warmup);
		r.iterations = 10000;
		results.push_back(r);
		printResult(r);
	}

	// Benchmark 11: Nested foreach with function calls
	if (specificBenchmark.empty() || specificBenchmark == "foreach_nested") {
		cout << "Running: Nested Foreach with Functions..." << endl;

		string tempDir = "projects/bench_foreach_nested";
		system(("mkdir -p " + tempDir).c_str());

		ofstream f(tempDir + "/main.xc");
		f << R"(
; Nested foreach with function calls stress test
function @process($val:number, $idx:number):number
	return $val * $idx + 1

init
	array $arr:number
	repeat 1000 ($i)
		$arr.append($i)
	var $total = 0
	foreach $arr ($idx, $value)
		$total += @process($value, $idx)
	output.0 ($total)
)";
		f.close();

		auto r = benchmarkProgram("Foreach + Functions (1k)", tempDir, samples, warmup);
		r.iterations = 1000;
		results.push_back(r);
		printResult(r);
	}

	// Print summary
	printSummary();

	if (csvOutput) {
		printResultsCSV();
	}

	return 0;
}
