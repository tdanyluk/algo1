#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>

using namespace std;

class Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using MillisecDuration = std::chrono::duration<double, std::milli>;

    std::string name_;
    TimePoint start_;
public:
    Timer(std::string name="")
    : name_(name)
    , start_(Clock::now())
    {}

    ~Timer()
    {
        TimePoint end = Clock::now();
        MillisecDuration millisec = end - start_;
        if(name_ != "")
        {
            std::cout << name_ << ": ";
        }
        std::cout << millisec.count() << " millisec" << std::endl;
    }
};

// Make the type changeable, try: float, double, long double
#ifndef FLOAT_TYPE_OVERRIDE
    using Float = long double;
#else
    using Float = FLOAT_TYPE_OVERRIDE;
#endif

Float randf(Float min=0, Float max=1)
{
    return Float(rand()) * (max-min) / RAND_MAX;
}

// Unnamed namespace makes the functions local to the file, so they can be fully inlined
// Otherwise 2 instances would be generated (one inline and one for external visibility)
namespace{

Float polinom1(const std::vector<Float>& z, Float x)
{
    assert(z.size() >= 1);
    // Leaving here an asm comment so we can identify the code fragment in the main.s asm file
    // when compiling with -save-temps
    asm volatile("# Polinom1 eleje");
    Float y = z[0];
    for(int i = 1; i<int(z.size()); i++)
    {
        Float h = x;
        for(int j = 1; j<i; j++)
        {
            h = h * x;
        }
        y = y + z[i] * h;
    }
    asm volatile("# Polinom1 vege");
    return y;
}

Float polinom2(const std::vector<Float>& z, Float x)
{
    assert(z.size() >= 1);
    asm volatile("# Polinom2 eleje");
    Float y = z[0];
    Float h = 1;
    for(int i = 1; i<int(z.size()); i++)
    {
        h = h * x;
        y = y + z[i] * h;
    }
    asm volatile("# Polinom2 vege");
    return y;
}

Float polinom3(const std::vector<Float>& z, Float x)
{
    assert(z.size() >= 1);
    asm volatile("# Polinom3 eleje");
    Float y = z[0];
    for(int i = 1; i<int(z.size()); i++)
    {
        y = y + z[i] * pow(x, i);
    }
    asm volatile("# Polinom3 vege");
    return y;
}

Float horner(const std::vector<Float>& z, Float x)
{
    assert(z.size() >= 1);
    asm volatile("# Horner eleje");
    Float y = z[z.size() - 1];
    for(int i = int(z.size()) - 2; i>=0; i--)
    {
        y = y * x + z[i];
    }
    asm volatile("# Horner vege");
    return y;
}

}

// The results of all function calls will be assigned to this variable
// We have to either print out or assign all results to a volatile variable,
// Otherwise the optimizer can just delete the calls.
// Printing out would take some time, so better just assign it.
volatile Float res;

int main()
{
    int n = 1000;
    std::vector<Float> z(n + 1);
    for(auto& f: z)
    {
        f = randf();
    }

    int repeats = 1000;
    std::vector<Float> xs(repeats);
    for(auto& f: xs)
    {
        f = randf();
    }

    {
        Timer t("Polinom1");
        for(int i = 0; i<repeats; i++)
        {
            res = polinom1(z, xs[i]);
        }
    }
    {
        Timer t("Polinom2");
        for(int i = 0; i<repeats; i++)
        {
            res = polinom2(z, xs[i]);
        }
    }
    {
        Timer t("Polinom3");
        for(int i = 0; i<repeats; i++)
        {
            res = polinom3(z, xs[i]);
        }
    }
    {
        Timer t("Horner");
        for(int i = 0; i<repeats; i++)
        {
            res = horner(z, xs[i]);
        }
    }
}
