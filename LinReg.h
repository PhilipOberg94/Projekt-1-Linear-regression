#pragma once

#include "vector.h"
#include "serial.h"

namespace ml{
    class LinReg {
    public:
    
        LinReg(const double &bias, const double &weight,
        const container::Vector<double> &trainingInput,
        const container::Vector<double> &trainingOutput,
        const double &learningRate = 0.01);
        
        
        double getBias() const;
        double getWeight() const;
        int getTrainingSetCount() const;
                
        double predict(const double &input) const;
        bool train(const int &epochs);
        
        
    private:
        double myBias;
        double myWeight;
        double myLearningRate;
        const container::Vector<double> myTrainingInput;
        const container::Vector<double> myTrainingOutput;
        
        
    };
}