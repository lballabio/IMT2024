#include "constantblackscholesprocess.hpp"

namespace QuantLib {

    ConstantBlackScholesProcess::ConstantBlackScholesProcess(
        double x0,
        Rate dividend,
        Rate riskFree,
        Volatility blackVol,
        const ext::shared_ptr<discretization>& disc)
        : StochasticProcess1D(disc), x0_(x0), riskFreeRate_(riskFree), dividendYield_(dividend), blackVolatility_(blackVol) {
        
        }

    /*
    Recall our stochastic process is written as :
    d\ln S(t) = (r - q - \frac{\sigma^2}{2}) dt + \sigma dW_t
    */

   // Get initial value
    Real ConstantBlackScholesProcess::x0() const {
        return x0_;
    }

    Real ConstantBlackScholesProcess::drift(Time t, Real x) const {
        Real sigma = diffusion(t,x);
        return riskFreeRate_ - dividendYield_ - 0.5 * sigma * sigma;
    }

    Real ConstantBlackScholesProcess::diffusion(Time t, Real x) const {
        return blackVolatility_;
    }

    Real ConstantBlackScholesProcess::apply(Real x0, Real dx) const {

        // We need to apply exp as the stochastic process is based on d\ln S(t)
        return x0 * std::exp(dx);
    }

   

}