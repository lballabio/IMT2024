#include "constantblackscholesprocess.hpp"

namespace QuantLib {

    ConstantBlackScholesProcess::ConstantBlackScholesProcess(
        Handle<Quote> x0,
        Handle<Quote> dividend,
        Handle<Quote> riskFree,
        Handle<Quote> blackVol,
        const ext::shared_ptr<discretization>& disc)
        : StochasticProcess1D(disc), x0_(std::move(x0)), riskFreeRate_(std::move(riskFree)), dividendYield_(std::move(dividend)), blackVolatility_(std::move(blackVol)) {
            registerWith(x0_);
            registerWith(riskFreeRate_);
            registerWith(dividendYield_);
            registerWith(blackVolatility_);
        }

    /*
    Recall our stochastic process is written as :
    d\ln S(t) = (r - q - \frac{\sigma^2}{2}) dt + \sigma dW_t
    */


    // NB : as we are using Handle<Quote>, we will have to use the value() method to retrieve their value

    Real ConstantBlackScholesProcess::x0() const {
        return x0_->value();
    }

    Real ConstantBlackScholesProcess::drift(Time t, Real x) const {
        Real sigma = diffusion(t,x);
        return riskFreeRate_->value() - dividendYield_->value() - 0.5 * sigma * sigma;
    }

    Real ConstantBlackScholesProcess::diffusion(Time t, Real x) const {
        return blackVolatility_->value();
    }

    Real ConstantBlackScholesProcess::apply(Real x0, Real dx) const {

        // We need to apply exp as the stochastic process is based on d\ln S(t)
        return x0 * std::exp(dx);
    }

   

}