/********************************************************************************
 * @brief Linear regression model implementation
 ********************************************************************************/
#pragma once

#include "vector.h"
#include "serial.h"

namespace ml 
{

/********************************************************************************
 * @brief Class for Linear Regression model
 ********************************************************************************/
class LinReg 
{
public:

    /********************************************************************************
     * @brief Constructor for Linear Regression model
     * 
     * @param bias Initial bias value
     * @param weight Initial weight value
     * @param trainingInput Vector of training input values
     * @param trainingOutput Vector of training output values
     * @param learningRate Learning rate for the model (default is 0.01)
     ********************************************************************************/
    LinReg(const double &bias, const double &weight,    
        const container::Vector<double> &trainingInput,     
        const container::Vector<double> &trainingOutput,
        const double &learningRate = 0.01);
    
    /********************************************************************************
     * @brief Get the current bias value
     * 
     * @return Current bias value
     ********************************************************************************/
    double getBias() const;

    /********************************************************************************
     * @brief Get the current weight value
     * 
     * @return Current weight value
     ********************************************************************************/
    double getWeight() const;

    /********************************************************************************
     * @brief Get the count of training sets
     * 
     * @return Number of training sets
     ********************************************************************************/
    int getTrainingSetCount() const;

    /********************************************************************************
     * @brief Predict the output for a given input using the linear regression model
     * 
     * @param input Input value for prediction
     * @return Predicted output value
     ********************************************************************************/
    double predict(const double &input) const;
    
    /********************************************************************************
     * @brief Train the linear regression model using the training data
     * 
     * @param epochs Number of epochs to train the model
     * @return True if training was successful, false otherwise
     ********************************************************************************/
    bool train(const int &epochs);

private:
    double myBias;                            
    double myWeight;                           
    double myLearningRate;                     
    const container::Vector<double> myTrainingInput;  
    const container::Vector<double> myTrainingOutput; 

};

} // namespace ml