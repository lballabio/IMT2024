/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2003, 2004 Neil Firth
 Copyright (C) 2003, 2004 Ferdinando Ametrano
 Copyright (C) 2003, 2004, 2005, 2007, 2008 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file mcbarrierengine.hpp
    \brief Monte Carlo barrier option engines
*/

#ifndef mc_barrier_engines_hpp
#define mc_barrier_engines_hpp

#include <ql/exercise.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/pricingengines/mcsimulation.hpp>
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <utility>

namespace QuantLib {

    //! Pricing engine for barrier options using Monte Carlo simulation
    /*! Uses the Brownian-bridge correction for the barrier found in
        <i>
        Going to Extremes: Correcting Simulation Bias in Exotic
        Option Valuation - D.R. Beaglehole, P.H. Dybvig and G. Zhou
        Financial Analysts Journal; Jan/Feb 1997; 53, 1. pg. 62-68
        </i>
        and
        <i>
        Simulating path-dependent options: A new approach -
        M. El Babsiri and G. Noel
        Journal of Derivatives; Winter 1998; 6, 2; pg. 65-83
        </i>

        \ingroup barrierengines

        \test the correctness of the returned value is tested by
              reproducing results available in literature.
    */
    template <class RNG = PseudoRandom, class S = Statistics>
    class MCBarrierEngine_2 : public BarrierOption::engine,
                              public McSimulation<SingleVariate,RNG,S> {
      public:
        typedef
        typename McSimulation<SingleVariate,RNG,S>::path_generator_type
            path_generator_type;
        typedef typename McSimulation<SingleVariate,RNG,S>::path_pricer_type
            path_pricer_type;
        typedef typename McSimulation<SingleVariate,RNG,S>::stats_type
            stats_type;
        // constructor
        MCBarrierEngine_2(ext::shared_ptr<GeneralizedBlackScholesProcess> process,
                          Size timeSteps,
                          Size timeStepsPerYear,
                          bool brownianBridge,
                          bool antitheticVariate,
                          Size requiredSamples,
                          Real requiredTolerance,
                          Size maxSamples,
                          bool isBiased,
                          BigNatural seed,
                          bool usingConstantParameters);
        void calculate() const override {
            Real spot = process_->x0();
            QL_REQUIRE(spot > 0.0, "negative or null underlying given");
            QL_REQUIRE(!triggered(spot), "barrier touched");
            McSimulation<SingleVariate,RNG,S>::calculate(requiredTolerance_,
                                                         requiredSamples_,
                                                         maxSamples_);
            results_.value = this->mcModel_->sampleAccumulator().mean();
            if (RNG::allowsErrorEstimate)
            results_.errorEstimate =
                this->mcModel_->sampleAccumulator().errorEstimate();
        }

      protected:
        // McSimulation implementation
        TimeGrid timeGrid() const override;
        
        ext::shared_ptr<path_generator_type> pathGenerator() const override;

        ext::shared_ptr<path_pricer_type> pathPricer() const override;
        // data members
        ext::shared_ptr<GeneralizedBlackScholesProcess> process_;
        Size timeSteps_, timeStepsPerYear_;
        Size requiredSamples_, maxSamples_;
        Real requiredTolerance_;
        bool isBiased_;
        bool brownianBridge_;
        BigNatural seed_;

        bool usingConstantParameters;
    };


    //! Monte Carlo barrier-option engine factory
    template <class RNG = PseudoRandom, class S = Statistics>
    class MakeMCBarrierEngine_2 {
      public:
        MakeMCBarrierEngine_2(ext::shared_ptr<GeneralizedBlackScholesProcess>);
        // named parameters
        MakeMCBarrierEngine_2& withSteps(Size steps);
        MakeMCBarrierEngine_2& withStepsPerYear(Size steps);
        MakeMCBarrierEngine_2& withBrownianBridge(bool b = true);
        MakeMCBarrierEngine_2& withAntitheticVariate(bool b = true);
        MakeMCBarrierEngine_2& withSamples(Size samples);
        MakeMCBarrierEngine_2& withAbsoluteTolerance(Real tolerance);
        MakeMCBarrierEngine_2& withMaxSamples(Size samples);
        MakeMCBarrierEngine_2& withBias(bool b = true);
        MakeMCBarrierEngine_2& withSeed(BigNatural seed);

        MakeMCBarrierEngine_2& withConstantParameters(bool b = true);
        // conversion to pricing engine
        operator ext::shared_ptr<PricingEngine>() const;
      private:
        ext::shared_ptr<GeneralizedBlackScholesProcess> process_;
        bool brownianBridge_ = false, antithetic_ = false, biased_ = false;
        Size steps_, stepsPerYear_, samples_, maxSamples_;
        Real tolerance_;
        BigNatural seed_ = 0;

        bool usingConstantParameters_;
    };


    // template definitions

    template <class RNG, class S>
    inline MCBarrierEngine_2<RNG, S>::MCBarrierEngine_2(
        ext::shared_ptr<GeneralizedBlackScholesProcess> process,
        Size timeSteps,
        Size timeStepsPerYear,
        bool brownianBridge,
        bool antitheticVariate,
        Size requiredSamples,
        Real requiredTolerance,
        Size maxSamples,
        bool isBiased,
        BigNatural seed,
        bool usingConstantParameters)
    : McSimulation<SingleVariate, RNG, S>(antitheticVariate, false), process_(std::move(process)),
      timeSteps_(timeSteps), timeStepsPerYear_(timeStepsPerYear), requiredSamples_(requiredSamples),
      maxSamples_(maxSamples), requiredTolerance_(requiredTolerance), isBiased_(isBiased),
      brownianBridge_(brownianBridge), seed_(seed), usingConstantParameters(usingConstantParameters) {
        QL_REQUIRE(timeSteps != Null<Size>() ||
                   timeStepsPerYear != Null<Size>(),
                   "no time steps provided");
        QL_REQUIRE(timeSteps == Null<Size>() ||
                   timeStepsPerYear == Null<Size>(),
                   "both time steps and time steps per year were provided");
        QL_REQUIRE(timeSteps != 0,
                   "timeSteps must be positive, " << timeSteps <<
                   " not allowed");
        QL_REQUIRE(timeStepsPerYear != 0,
                   "timeStepsPerYear must be positive, " << timeStepsPerYear <<
                   " not allowed");
        registerWith(process_);
    }

    template <class RNG, class S>
    inline TimeGrid MCBarrierEngine_2<RNG,S>::timeGrid() const {

        Time residualTime = process_->time(arguments_.exercise->lastDate());
        if (timeSteps_ != Null<Size>()) {
            return TimeGrid(residualTime, timeSteps_);
        } else if (timeStepsPerYear_ != Null<Size>()) {
            Size steps = static_cast<Size>(timeStepsPerYear_*residualTime);
            return TimeGrid(residualTime, std::max<Size>(steps, 1));
        } else {
            QL_FAIL("time steps not specified");
        }
    }


    template <class RNG, class S>
    inline
    ext::shared_ptr<typename MCBarrierEngine_2<RNG,S>::path_pricer_type>
    MCBarrierEngine_2<RNG,S>::pathPricer() const {
        ext::shared_ptr<PlainVanillaPayoff> payoff =
            ext::dynamic_pointer_cast<PlainVanillaPayoff>(arguments_.payoff);
        QL_REQUIRE(payoff, "non-plain payoff given");

        TimeGrid grid = timeGrid();
        std::vector<DiscountFactor> discounts(grid.size());
        for (Size i=0; i<grid.size(); i++)
            discounts[i] = process_->riskFreeRate()->discount(grid[i]);

        // do this with template parameters?
        if (isBiased_) {
            return ext::shared_ptr<
                        typename MCBarrierEngine_2<RNG,S>::path_pricer_type>(
                new BiasedBarrierPathPricer(
                       arguments_.barrierType,
                       arguments_.barrier,
                       arguments_.rebate,
                       payoff->optionType(),
                       payoff->strike(),
                       discounts));
        } else {
            PseudoRandom::ursg_type sequenceGen(grid.size()-1,
                                                PseudoRandom::urng_type(5));
            return ext::shared_ptr<
                        typename MCBarrierEngine_2<RNG,S>::path_pricer_type>(
                new BarrierPathPricer(
                    arguments_.barrierType,
                    arguments_.barrier,
                    arguments_.rebate,
                    payoff->optionType(),
                    payoff->strike(),
                    discounts,
                    process_,
                    sequenceGen));
        }
    }

    template <class RNG, class S>
    inline ext::shared_ptr<typename MCBarrierEngine_2<RNG,S>::path_generator_type>
    MCBarrierEngine_2<RNG,S>::pathGenerator() const {
        TimeGrid grid = timeGrid();
        typename RNG::rsg_type gen =
            RNG::make_sequence_generator(grid.size()-1,seed_);

        if (this->usingConstantParameters) {
            Time currentTime = grid.back();
            double spot = this->process_->x0();
            double riskFreeRate = this->process_->riskFreeRate()->zeroRate(currentTime, Continuous);
            double dividendYield = this->process_->dividendYield()->zeroRate(currentTime, Continuous);
            double volatility = this->process_->blackVolatility()->blackVol(currentTime, spot);

            ext::shared_ptr<ConstantBlackScholesProcess> constantProcess(new ConstantBlackScholesProcess(spot, riskFreeRate, volatility, dividendYield));

            return ext::shared_ptr<typename MCBarrierEngine_2<RNG,S>::path_generator_type>(
                new path_generator_type(constantProcess, grid, gen, this->brownianBridge_));
        } else {
            
            return ext::shared_ptr<typename MCBarrierEngine_2<RNG,S>::path_generator_type>(
                new path_generator_type(this->process_, grid, gen, this->brownianBridge_));
        }
}

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG, S>::MakeMCBarrierEngine_2(
        ext::shared_ptr<GeneralizedBlackScholesProcess> process)
    : process_(std::move(process)), steps_(Null<Size>()), stepsPerYear_(Null<Size>()),
      samples_(Null<Size>()), maxSamples_(Null<Size>()), tolerance_(Null<Real>()) {}

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withSteps(Size steps) {
        steps_ = steps;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withStepsPerYear(Size steps) {
        stepsPerYear_ = steps;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withBrownianBridge(bool brownianBridge) {
        brownianBridge_ = brownianBridge;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withAntitheticVariate(bool b) {
        antithetic_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withSamples(Size samples) {
        QL_REQUIRE(tolerance_ == Null<Real>(),
                   "tolerance already set");
        samples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withAbsoluteTolerance(Real tolerance) {
        QL_REQUIRE(samples_ == Null<Size>(),
                   "number of samples already set");
        QL_REQUIRE(RNG::allowsErrorEstimate,
                   "chosen random generator policy "
                   "does not allow an error estimate");
        tolerance_ = tolerance;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withMaxSamples(Size samples) {
        maxSamples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withBias(bool biased) {
        biased_ = biased;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withSeed(BigNatural seed) {
        seed_ = seed;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCBarrierEngine_2<RNG,S>&
    MakeMCBarrierEngine_2<RNG,S>::withConstantParameters(bool b) {
        usingConstantParameters_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline
    MakeMCBarrierEngine_2<RNG,S>::operator ext::shared_ptr<PricingEngine>() const {
        QL_REQUIRE(steps_ != Null<Size>() || stepsPerYear_ != Null<Size>(),
                   "number of steps not given");
        QL_REQUIRE(steps_ == Null<Size>() || stepsPerYear_ == Null<Size>(),
                   "number of steps overspecified");
        return ext::shared_ptr<PricingEngine>(new
            MCBarrierEngine_2<RNG,S>(process_,
                                     steps_,
                                     stepsPerYear_,
                                     brownianBridge_,
                                     antithetic_,
                                     samples_, tolerance_,
                                     maxSamples_,
                                     biased_,
                                     seed_,
                                     usingConstantParameters_));
    }

}


#endif
