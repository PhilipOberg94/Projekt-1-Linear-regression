#include "LinReg.h"



namespace ml{
    
    LinReg::LinReg(const double &bias, const double &weight,
    const container::Vector<double> &trainingInput,
    const container::Vector<double> &trainingOutput,
    const double &learningRate)
    : myBias(bias)
    , myWeight(weight)
    , myLearningRate(learningRate)
    , myTrainingInput(trainingInput)
    , myTrainingOutput(trainingOutput)
    {
    }
    
    double LinReg::getBias() const                      # Getters for bias, weight, trainingsetcount
    {
        return myBias;
    }

    double LinReg::getWeight() const
    {
        return myWeight;
    }

    int LinReg::getTrainingSetCount() const
    {
        return myTrainingInput.size();
    }

    double LinReg::predict(const double &input) const   // Prediction of the model
    {
        return myBias + myWeight * input;
    }
    
    bool LinReg::train(const int &epochs)               // How many times the training data is used to train the model
    {
        double prediction{};
        double error{};
            
        if (epochs == 0 || myLearningRate <= 0) { return false; }
            
        for (int i = 0; i < epochs; i++)
        {
            for (auto j = 0U; j < myTrainingInput.size(); j++)
            {
                const auto &ref(myTrainingOutput[j]);
                const auto &input(myTrainingInput[j]);
                
                if(input == 0){
                    myBias = ref;
                }
                else{
                    prediction = predict(input);
                    error = ref - prediction;

                    myBias += error * myLearningRate;
                    myWeight += error * myLearningRate * input;
                }                
            }
        }
        return true;
    }
}