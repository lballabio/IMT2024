
#include <ql/stochasticprocess.hpp>
#include <ql/quote.hpp>
#include <ql/processes/eulerdiscretization.hpp>

/*
Implements interface StochasticProcess1D
Why it does not inheritate from GeneralizedBlackScholesProcess ?
Because most of GeneralizedBlackScholesProcess methods can't be reused for our constant black-scholes process... ?
*/

namespace QuantLib {

    class ConstantBlackScholesProcess : public StochasticProcess1D {

        /*
        StochasticProcess1D : dx_t = \mu(t, x_t)dt + \sigma(t, x_t)dW_t
        To build this class, I based on GeneralizedBlackScholesProcess : d\ln S(t) = (r(t) - q(t) - \frac{\sigma(t, S)^2}{2}) dt + \sigma dW_t
        Here however, r, q and \sigma are constant
        */

    public:
        ConstantBlackScholesProcess(

            /*
            Handle : copies of a given handle share a link to an object; when the link is made to point to another object, all copies are notified and
            allow their holders to access the new pointee. Moreover, it forwards any notification from the pointed objects to their observers

            I use Handle here, to be consistent with other classes that implement StochasticProcess1D / StochasticProcess
            (see GeneralizedBlackScholesProcess for instance)
            Is it useful here ? No if this class purpose is only to be used in the MC Engines, surely yes otherwise...
            */

            Handle<Quote> x0,           // underlying initial value
            Handle<Quote> dividend,     // dividend yield
            Handle<Quote> riskFree,     // risk-free rate
            Handle<Quote> blackVol,     // volatility
            const ext::shared_ptr<discretization>& d = ext::shared_ptr<discretization>(new EulerDiscretization)

            /*
            discretization : inner class of StochasticProcess1D; enables to change the way a process is discretly sampled
            Default discretization way is Euler method (as for GeneralizedBlackScholesProcess)
            EulerDiscretization enables to compute change of stochastic process in dt
            */
        );

        /*
        We need to implement drift, diffusion, apply and stdDeviation methods as they appear in evolve method
        evolve method is essential as it enables to compute the asset value after a time interval dt according to the given discretization
        It is thus the main component to generate the paths in a MC simulation

        Indeed, the way random paths are generated go back to MonteCarloModel class (present in McSimulation, in turn present in each MC engine)
        with a variable of type PathGenerator. Also notice that the path generator type is set in the class McSimulation through the method pathGenerator()
        This class PathGenerator has a method next that computes the next point of a random path, and uses the methods x0 and evolve

        Notice this variable of type PathGenerator is the only variable of interest in our MonteCarloModel class as the other variables do not
        relate to the characteristics of the asset underlying stochastic process
        For instance, the variable of type PathPricer, is only responsible for pricing the paths, this operation being independent from the stochastic process
        */

        Real x0() const override;
        Real drift(Time t, Real x) const override;
        Real diffusion(Time t, Real x) const override;
        Real apply(Real x0, Real dx) const override;
        Real stdDeviation(Time t0, Real x0, Time dt) const override;
        Real evolve(Time t0, Real x0, Time dt, Real dw) const override;
    

    private:
        Handle<Quote> x0_;
        Handle<Quote> riskFreeRate_, dividendYield_;
        Handle<Quote> blackVolatility_;
    
    };

}



