#!/usr/bin/env python3
"""
Python Benchmark - Equivalent to XenonCode benchmark.cpp
Mirrors the same computational tests for fair comparison.
"""

import time
import math
import statistics
from typing import List, Callable, Any
from dataclasses import dataclass, field


@dataclass
class BenchmarkResult:
    name: str
    samples_ms: List[float] = field(default_factory=list)
    iterations: int = 1

    def min_time(self) -> float:
        return min(self.samples_ms) if self.samples_ms else 0

    def max_time(self) -> float:
        return max(self.samples_ms) if self.samples_ms else 0

    def avg(self) -> float:
        return sum(self.samples_ms) / len(self.samples_ms) if self.samples_ms else 0

    def stddev(self) -> float:
        if len(self.samples_ms) < 2:
            return 0
        return statistics.stdev(self.samples_ms)

    def per_iteration_us(self) -> float:
        return (self.avg() * 1000.0) / self.iterations


results: List[BenchmarkResult] = []


def benchmark(name: str, func: Callable, samples: int = 10, warmup: int = 3, iterations: int = 1) -> BenchmarkResult:
    """Run a benchmark with warmup and multiple samples."""
    result = BenchmarkResult(name=name, iterations=iterations)

    # Warmup runs
    for _ in range(warmup):
        func()

    # Benchmark runs
    for _ in range(samples):
        start = time.perf_counter()
        func()
        elapsed_ms = (time.perf_counter() - start) * 1000
        result.samples_ms.append(elapsed_ms)

    return result


def print_result(r: BenchmarkResult):
    print(f"{r.name:<35}{r.avg():>12.3f} ms{r.min_time():>12.3f} ms (min){r.max_time():>12.3f} ms (max){r.stddev():>12.3f} ms (std)")


def print_summary():
    print("\n=== Benchmark Summary ===\n")
    print(f"{'Benchmark':<35}{'Avg':>15}{'Min':>15}{'Max':>15}{'Stddev':>15}")
    print("-" * 95)
    for r in results:
        print_result(r)


# Benchmark 1: Math Operations (100k iterations)
def bench_math():
    result = 0.0
    for i in range(100000):
        x = i * 3.14159
        y = x / 2.71828
        z = y + x - 1.0
        result = z * math.sin(x) + math.cos(y)
    return result


# Benchmark 2: Array Operations (10k append)
def bench_array():
    arr = []
    for i in range(10000):
        arr.append(i)
    total = sum(arr)
    avg = total / len(arr)
    max_val = max(arr)
    min_val = min(arr)
    return total, avg, max_val, min_val


# Benchmark 3: Text Operations (1k)
def bench_text():
    text = ""
    for i in range(1000):
        text = f"{i:06d}"
        upper = text.upper()
        lower = upper.lower()
        length = len(lower)
    return text


# Benchmark 4: Key-Value Access (10k) - String parsing like XenonCode's text key-value objects
def get_kv_value(obj: str, key: str) -> str:
    """Parse .key{value} format like XenonCode does"""
    pattern = f".{key}{{"
    idx = obj.find(pattern)
    if idx == -1:
        # Case-insensitive fallback
        idx = obj.lower().find(pattern.lower())
    if idx == -1:
        return ""
    start = idx + len(pattern)
    end = start
    depth = 0
    while end < len(obj):
        c = obj[end]
        if c == '{':
            depth += 1
        elif c == '}':
            if depth == 0:
                break
            depth -= 1
        end += 1
    return obj[start:end]

def bench_keyvalue():
    obj = ".name{test}.value{42}.x{1.5}.y{2.5}.z{3.5}"
    result = 0
    for _ in range(10000):
        name = get_kv_value(obj, "name")
        val = get_kv_value(obj, "value")
        x = get_kv_value(obj, "x")
        y = get_kv_value(obj, "y")
        z = get_kv_value(obj, "z")
        result += 1
    return result


# Benchmark 5: Foreach Loop (10k)
def bench_foreach():
    arr = []
    for i in range(10000):
        arr.append(i * 0.5)
    total = 0.0
    for idx, value in enumerate(arr):
        total += value * 2
    return total


# Benchmark 6: Foreach + Functions (1k)
def process(val: float, idx: int) -> float:
    return val * idx + 1

def bench_foreach_functions():
    arr = list(range(1000))
    total = 0.0
    for idx, value in enumerate(arr):
        total += process(value, idx)
    return total


# Benchmark 7: Device function simulation (100k) - Simple function calls
def benchmark_return_num() -> float:
    return 42.0

def bench_device_calls():
    result = 0.0
    for _ in range(100000):
        x = benchmark_return_num()
        result += x
    return result


# Benchmark 8: Device function with args (100k)
def benchmark_with_args(a: float, b: float) -> float:
    return a + b

def bench_device_args():
    result = 0.0
    for _ in range(100000):
        x = benchmark_with_args(1.5, 2.5)
        result += x
    return result


# Benchmark 9: Object member access simulation (50k)
class Position:
    def __init__(self):
        self.x = 1.0
        self.y = 2.0
        self.z = 3.0

def bench_object_access():
    result = 0.0
    for _ in range(50000):
        pos = Position()
        x = pos.x
        y = pos.y
        z = pos.z
        result = x + y + z
    return result


def main():
    samples = 10
    warmup = 3

    print("Python Benchmark Tool")
    print("=====================\n")
    print(f"Configuration: {samples} samples, {warmup} warmup runs\n")

    # Run benchmarks
    print("Running: Math Operations...")
    r = benchmark("Math Operations (100k)", bench_math, samples, warmup, 100000)
    results.append(r)
    print_result(r)

    print("Running: Array Operations...")
    r = benchmark("Array Operations (10k append)", bench_array, samples, warmup, 10000)
    results.append(r)
    print_result(r)

    print("Running: Text Operations...")
    r = benchmark("Text Operations (1k)", bench_text, samples, warmup, 1000)
    results.append(r)
    print_result(r)

    print("Running: Key-Value Access...")
    r = benchmark("Key-Value Access (10k)", bench_keyvalue, samples, warmup, 10000)
    results.append(r)
    print_result(r)

    print("Running: Foreach Loop...")
    r = benchmark("Foreach Loop (10k)", bench_foreach, samples, warmup, 10000)
    results.append(r)
    print_result(r)

    print("Running: Foreach + Functions...")
    r = benchmark("Foreach + Functions (1k)", bench_foreach_functions, samples, warmup, 1000)
    results.append(r)
    print_result(r)

    print("Running: Function Calls (100k)...")
    r = benchmark("Function Calls (100k)", bench_device_calls, samples, warmup, 100000)
    results.append(r)
    print_result(r)

    print("Running: Function Calls with Args (100k)...")
    r = benchmark("Function Calls with Args (100k)", bench_device_args, samples, warmup, 100000)
    results.append(r)
    print_result(r)

    print("Running: Object Access...")
    r = benchmark("Object Access (50k)", bench_object_access, samples, warmup, 50000)
    results.append(r)
    print_result(r)

    print_summary()


if __name__ == "__main__":
    main()
