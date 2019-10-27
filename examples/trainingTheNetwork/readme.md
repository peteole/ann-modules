# Example for training a neural network
Again, I will use the network from the first example.
## The training task
As an example, the network is given ten random numbers beween 0 and 1. The three target outputs are the average of three of the inputs each, so one input does not need to be used. Therefore, the creation of a training example looks like the following:
```cpp
for (int j = 0; j < 10; j++) {
  input[j] = 1 * (double) random() / (double) RAND_MAX;
}
for(int j=0;j<3;j++){
  target[j]=(input[3*j]+input[3*j+1]+input[3*j+2])/3;
}
```
## evaluating a training example
The activations and the derivatives have to be computed:
```cpp
container.updateOutput(input, target);
container.addDerivatives();
```
## updating the parameters
To reduce noise, the previous steps are repeated a few (50) times before the parameters are changed in accordance with the sum of the derivatives. A learning rate of -1 proved to be the fastest in this context.
```cpp
container.updateParameters(-1);
```
## putting it all together
The code for the learning is the following:
```cpp	
//create array for the training inputs
double *input = new double[10];
double *target=new double[3];
for (int i = 0; i < 800000; i++) {
  for (int k = 0; k < 5; k++) {
    for (int j = 0; j < 10; j++) {
      input[j] = 1 * (double) random() / (double) RAND_MAX;
    }
    for(int j=0;j<3;j++){
      target[j]=(input[3*j]+input[3*j+1]+input[3*j+2])/3;
    }
    container.updateOutput(input, target);
    container.addDerivatives();
  }
  cout<<container.value<<endl;
  container.updateParameters(-1);
}
```
