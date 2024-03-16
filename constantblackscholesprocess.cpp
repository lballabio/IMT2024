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

    Real ConstantBlackScholesProcess::x0() const {

        // As the underlying class of x0_ is Quote, we get the value of a Quote using toto->value()
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

    Real ConstantBlackScholesProcess::stdDeviation(Time t0, Real x0, Time dt) const {
            return discretization_->diffusion(*this, t0, x0, dt);
    }

    Real ConstantBlackScholesProcess::evolve(Time t0, Real x0, Time dt, Real dw) const {
        return apply(
            x0,
            discretization_->drift(*this, t0, x0, dt) + stdDeviation(t0, x0, dt) * dw
        );
    }

}