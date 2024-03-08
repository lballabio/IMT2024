#include "constantblackscholesprocess.hpp"
#include <ql/processes/eulerdiscretization.hpp>

constantblackscholesprocess::constantblackscholesprocess() :StochasticProcess1D(ext::make_shared<EulerDiscretization>()) {
    spot = 100.;
    rf_rate = 0.05;
    dividend = 3.;
    volatility = 1.;
}

constantblackscholesprocess::constantblackscholesprocess(constantblackscholesprocess &process_ref) :StochasticProcess1D(ext::make_shared<EulerDiscretization>()){
    spot = process_ref.getSpot();
    rf_rate = process_ref.getRf_rate();
    dividend = process_ref.getDividend();
    volatility = process_ref.getVolatility();
}

constantblackscholesprocess::constantblackscholesprocess(double spot1, double rf_rate1, double dividend1,
                                                         double volatility1)  :StochasticProcess1D(ext::make_shared<EulerDiscretization>()){
    spot = spot1;
    rf_rate = rf_rate1;
    dividend = dividend1;
    volatility = volatility1;
}

constantblackscholesprocess::~constantblackscholesprocess() {

}

double constantblackscholesprocess::getSpot() const {
    return spot;
}

double constantblackscholesprocess::getRf_rate() const {
    return rf_rate;
}

double constantblackscholesprocess::getDividend() const {
    return dividend;
}

double constantblackscholesprocess::getVolatility() const {
    return volatility;
}

Real constantblackscholesprocess::x0() const {
    return getSpot();
}

Real constantblackscholesprocess::drift(Time t, Real x) const {
    return this->getRf_rate() - this->getDividend() - 0.5 * getVolatility() * getVolatility();
}

Real constantblackscholesprocess::diffusion(Time t, Real x) const {
    return this->getVolatility();
}

Real constantblackscholesprocess::apply(Real x0, Real dx) const {
    return x0 * std::exp(dx);
}

