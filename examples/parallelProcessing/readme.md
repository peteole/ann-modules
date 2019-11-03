# using multithreading to boost performance
There is a toolset included to train your networks using multiple processor cores. To use it, you must include training.h.
## The structure of a set of training examples
Training examples are stored in a 3d array. It follows the following scheme if your set of examples is called t
* t[i] is the i-th training example
* t[i][0] is the array of inputs for the i-th example
* t[i][1] is the array of target outputs for the i-th example
So an example set of training examples for a network with 10 inputs and 3 outputs can be created like this:
```cpp
#define numOfExamples 1000
double ***trainingExamples = new double**[numOfExamples];
for (int i = 0; i < numOfExamples; i++) {
  trainingExamples[i] = new double*[2];
  trainingExamples[i][0] = new double[10];
  trainingExamples[i][1] = new double[3];
  double *input = trainingExamples[i][0];
  for (int j = 0; j < 10; j++) {
    input[j] = 1 * (double) random() / (double) RAND_MAX;
  }
  for (int j = 0; j < 3; j++) {
    trainingExamples[i][1][j] = (input[3 * j] + input[3 * j + 1]
        + input[3 * j + 2]) / 3;
  }
}
```
## calculating the derivatives
This is the point where the multithreading starts to take effect: Just use the train-method to calculate the gradients of the given network for the given set of training examples with the given number of examples and the given number of threads you want to use:
```cpp
//use 1000 examples and 4 threads
train(&container, trainingExamples, 1000, 4);
```
Note that to use this feature, you must construct your container by giving a pointer to a construction method: https://github.com/peteole/ann-modules/tree/master/examples/copyingNetworkContainers
## updating the parameters
This is business as usual:
```cpp
container.updateParameters(-0.001);
```
Of course you can specify any training method you wish: https://github.com/peteole/ann-modules/tree/master/examples/costumTrainingMethod
