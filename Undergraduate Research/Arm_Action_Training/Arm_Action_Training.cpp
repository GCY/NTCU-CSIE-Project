#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <time.h>

// OpenNN includes

#include "opennn.h"

using namespace OpenNN;

int main(int argc, char** argv)
{
   int inputNumber = 4;
   int neuronNumber = 0;
   int outputNumber = 0;
   
   if(atoi(argv[1])>0 && atoi(argv[2])>0){
	   neuronNumber = atoi(argv[1]);
	   outputNumber = atoi(argv[2]);
   }
   
   try
   {
      std::cout << "Arm action recognition Application." << std::endl;

      srand((unsigned)time(NULL));

      // Data set 
  
      DataSet data_set;

      data_set.load_data("data/arm_action.dat"); // load dataset

      // Variables information

      VariablesInformation* variables_information_pointer = data_set.get_variables_information_pointer(); 

	  const Vector<unsigned int> inputs_indices(0, 1, inputNumber-1); // 0~inputNumber-1 為輸入
	  const Vector<unsigned int> targets_indices(inputNumber, 1, inputNumber+outputNumber-1); // inputNumber ~ inputNumber+outputNumber-1 為目標

	  variables_information_pointer->set_inputs_indices(inputs_indices);
	  variables_information_pointer->set_targets_indices(targets_indices);

      variables_information_pointer->set_name(0, "leftElbow"); // 左手肘角度
      variables_information_pointer->set_units(0, "degree");
       
      variables_information_pointer->set_name(1, "leftShoulder"); // 左肩膀角度
      variables_information_pointer->set_units(1, "degree");
       
      variables_information_pointer->set_name(2, "rightElbow"); // 右手肘角度
      variables_information_pointer->set_units(2, "degree");

      variables_information_pointer->set_name(3, "rightShoulder"); // 右肩膀角度
      variables_information_pointer->set_units(3, "degree");

	  for(int i=inputNumber; i<inputNumber+outputNumber; i++){
		  char str[10];
		  sprintf(str, "%s%i", "action_", i);
	      variables_information_pointer->set_name(i, str); //設定目標輸出
	  }

      const Vector< Vector<std::string> > inputs_targets_information = variables_information_pointer->arrange_inputs_targets_information();

	  // Instances information
    
      InstancesInformation* instances_information_pointer = data_set.get_instances_information_pointer(); 

      // 訓練75% 測試25%
      instances_information_pointer->split_random_indices(75.0, 0.0, 25.0);
   
      const Vector< Vector<double> > inputs_targets_statistics = data_set.scale_inputs_minimum_maximum();
       
      const Vector< Vector<double> > variables_statistics = data_set.scale_inputs();


      // Neural network

      NeuralNetwork neural_network(inputNumber, neuronNumber, outputNumber);
									//輸入源數    神經元數     目標類別數     

      neural_network.set_inputs_outputs_information(inputs_targets_information);
      
	  neural_network.set_inputs_outputs_statistics(inputs_targets_statistics);

	  ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();

      neural_network.set_scaling_unscaling_layers_flag(false);

      // Performance functional

      PerformanceFunctional performance_functional(&neural_network, &data_set);

      // Training strategy

      TrainingStrategy training_strategy(&performance_functional);

      TrainingStrategy::Results training_strategy_results = training_strategy.perform_training();

      neural_network.set_scaling_layer_flag(true);
       //scaling_layer_pointer->set_scaling_method(ScalingLayer::MeanStandardDeviation);

      // Pattern recognition testing
                  
      TestingAnalysis testing_analysis(&neural_network, &data_set);

	  testing_analysis.construct_pattern_recognition_testing();

	  PatternRecognitionTesting* pattern_recognition_testing_pointer = testing_analysis.get_pattern_recognition_testing_pointer();

      // Save results

      data_set.save("result/data_set.xml");

      neural_network.save("result/neural_network.xml");
      neural_network.save_expression("result/expression.txt");   

	  training_strategy.save("result/training_strategy.xml");
	  training_strategy_results.save("result/training_strategy_results.dat");

      pattern_recognition_testing_pointer->save_confusion("result/confusion.dat");

      return(0);
   }
   catch(std::exception& e)
   {
      std::cout << e.what() << std::endl;		 

      return(1);
   }

}  