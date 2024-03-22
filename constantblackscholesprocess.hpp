
// constantblackscholesprocess.hpp
#ifndef CONSTANTBLACKSCHOLES_PROCESS_HPP
#define CONSTANTBLACKSCHOLES_PROCESS_HPP

#include <ql/stochasticprocess.hpp>
#include <ql/processes/eulerdiscretization.hpp>

using namespace QuantLib;

class constantblackscholesprocess : public StochasticProcess1D {
private:
    double spot;
    double rf_rate;
    double dividend;
    double volatility;

public:
    constantblackscholesprocess();
    constantblackscholesprocess(const constantblackscholesprocess &process_ref);
    constantblackscholesprocess(double spot1, double rf_rate1, double dividend1, double volatility1);
    ~constantblackscholesprocess();

    double getSpot() const;
    double getRf_rate() const;
    double getDividend() const;
    double getVolatility() const;

    Real x0() const;
    Real drift(Time t, Real x) const;
    Real diffusion(Time t, Real x) const;
    Real apply(Real x0, Real dx) const;
};

#endif // CONSTANTBLACKSCHOLES_PROCESS_HPP
