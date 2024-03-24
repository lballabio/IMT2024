
#ifndef _CALCULATE_NPV_H
#define _CALCULATE_NPV_H

#include <ql/processes/blackscholesprocess.hpp>
#include <boost/shared_ptr.hpp>
#include <ql/quantlib.hpp>
#include <iostream>
#include <chrono> // Include chrono for time measurement


namespace QuantLib{

    // Struct to hold NPV error calculation and time difference between the non constant and constant cases
    struct NPVErrorTime {
        Real errorConstantParams;
        Real errorNonConstantParams;
        double time;
    };

    template <class OptionType, class NewMakeMCEngine, class OldMakeMCEngine>

    inline ext::shared_ptr<NPVErrorTime> calculate_npv_time(
        OptionType& option,
        ext::shared_ptr<BlackScholesProcess>& bsmProcess,
        Size timeSteps,
        Size samples,
        Size mcSeed)
    {
        // Calculate NPV with old engines
        option.setPricingEngine(
            OldMakeMCEngine(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
        );

        Real oldNPV = option.NPV();

        // Calculate NPV with non-constant parameters
        option.setPricingEngine(
            NewMakeMCEngine(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(false)
        );

        auto startTime1 = std::chrono::steady_clock::now();
        Real nonConstantNPV = option.NPV();
        auto endTime1 = std::chrono::steady_clock::now();
        double durationNonConstant = std::chrono::duration_cast<std::chrono::microseconds>(endTime1 - startTime1).count() / 1000000.0;

        // Calculate NPV with constant parameters
        option.setPricingEngine(
            NewMakeMCEngine(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(samples)
            .withSeed(mcSeed)
            .withConstantParameters(true)
        );
        
        auto startTime2 = std::chrono::steady_clock::now();
        Real constantNPV = option.NPV();
        auto endTime2 = std::chrono::steady_clock::now();
        double durationConstant = std::chrono::duration_cast<std::chrono::microseconds>(endTime2 - startTime2).count() / 1000000.0;

        // Calculate difference in time and error
        double diffTime = durationNonConstant - durationConstant;
        double errorConstantParams = std::abs(constantNPV - oldNPV);
        double errorNonConstantParams = std::abs(nonConstantNPV - oldNPV);


        // Create and return NPVErrorTime struct
        auto result = ext::make_shared<NPVErrorTime>();
        result->errorConstantParams = errorConstantParams;
        result->errorNonConstantParams = errorNonConstantParams;
        result->time = diffTime;
        return result;
        
    }
    
}

#endif