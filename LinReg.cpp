#include "LinReg.h" 

namespace ml 
{

/********************************************************************************
 * @brief The constructor of the linear regression model
 * 
 * @param bias Initial bias value
 * @param weight Initial weight value
 * @param trainingInput Vector of training input values
 * @param trainingOutput Vector of training output values
 * @param learningRate Learning rate for the model
 ********************************************************************************/
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

/********************************************************************************
 * @brief Get the current bias value
 * 
 * @return Current bias value
 ********************************************************************************/
double LinReg::getBias() const
{
    return myBias;
}

/********************************************************************************
 * @brief Get the current weight value
 * 
 * @return Current weight value
 ********************************************************************************/
double LinReg::getWeight() const
{
    return myWeight;
}

/********************************************************************************
 * @brief Get the count of training sets
 * 
 * @return Number of training sets
 ********************************************************************************/
int LinReg::getTrainingSetCount() const
{
    return myTrainingInput.size();
}

/********************************************************************************
 * @brief Predict the output for a given input using the linear regression model
 * 
 * @param input Input value for prediction
 * @return Predicted output value
 ********************************************************************************/
double LinReg::predict(const double &input) const
{
    return myBias + myWeight * input;
}

/********************************************************************************
 * @brief Train the linear regression model using the training data
 * 
 * @param epochs Number of epochs to train the model
 * @return True if training was successful, false otherwise
 ********************************************************************************/
bool LinReg::train(const int &epochs)
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

} // namespace ml