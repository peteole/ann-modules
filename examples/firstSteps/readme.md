# Example of a basic neural network being created
This is the peaces of code needed to create a basic connection of neural networks. The network shall be the following:
```

|-------------- 0-- |----------\ 
|               1-- |           0-- |---------\
|               2-- | network1  1-- |           \
|               3-- |          /    |             0-- |---------- 0-- |-------------- |
|               4-- |---------/     | network3    1-- | errorDef  1-- | container.out |
| container.in  5-- |----------\    |             2-- |---------- 2-- |-------------- |
|               6-- |           0-- |            /
|               7-- | network2  1-- |----------/
|               8-- |          /
|-------------- 9-- |---------/
```
The steps are the following:
## create the container for the networks
```
NetworkContainer container(10, 3, 3);
```
## create the networks
```
FullyConnectedNetwork *network1 = new FullyConnectedNetwork(new int[3] { 5, 4, 2 }, 3);
FullyConnectedNetwork *network2 = new FullyConnectedNetwork(new int[3] { 5, 3, 2 }, 3);
FullyConnectedNetwork *network3 = new FullyConnectedNetwork(new int[2] { 4, 3 }, 2);
ValueDefiner *errorDef = new ErrorDefiner(3, &container, 0);
```
## add the networks to the container
```
container.addNeuralNetwork(network1);
container.addNeuralNetwork(network2);
container.addNeuralNetwork(network3);
container.addNeuralNetwork(errorDef);
```
## connect the networks to each other
```
container.makeNetworkChildOf(network1, &(container.in), 0);
container.makeNetworkChildOf(network2, &(container.in), 5);
container.makeNetworkChildOf(network3, network1, 0, 2, 0);
container.makeNetworkChildOf(network3, network2, 0, 2, 2);
container.makeNetworkChildOf(errorDef, network3, 0);
container.makeNetworkChildOf(&(container.out), errorDef);
```
## let the container determine in which order it has to let the networks evaluate their inputs
```
container.makeOrder();
```
## compute and print an output and error value for a given training example(input, target output)
```
container.updateOutput(new double[10]{1,4,-2,3,5,2,8,2,4,1}, new double[3] { 1, 4, 2 });
container.printOutput();
```
## compute the parameter derivatives for the last training example
```
container.addDerivatives();
```
## change parameters by -0.01 times the error value derivatives and reset error value derivatives
```
container.updateParameters(-0.01);
```
