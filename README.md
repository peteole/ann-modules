# ann-modules
A modular system to create and train your own custom neural networks
## installation
Just copy the src folder to your working directory.
## The classes used
The idea is to create a set of components you can connect to each other as you wish.
### PlugOut, PlugIn
You can imagine these classes just as a pair of mechanical plugs you can connect to each other: 
The PlugOut provides access to the output of a neural network and the PlugIn can recieve input if connected to a PlugOut.
### NeuralNetwork
This is an abstraction of a neural network. It is a child of PlugIn and PlugOut as it needs to recieve input and has to provide an output.  It has abstract methods to evaluate an input, a training example and to update its parameters accoarding to the last training examples using gradient descent.
### FullyConnectedNetwork
An implementation of a neural network implementing backpropagation.
### NetworkContainer
A neural network which can include several neural networks. You can add networks to the container using the addNetwork-Method. Then use the makeNetworkChildOf-Method of the container to connect the networks in the container as you wish. When finished, use the makeOrder-Method to let the NetworkContainer evaluate in which order the updateOutput-Methods need to be called to evaluate an input. For more information see https://github.com/peteole/ann-modules/tree/master/examples/firstSteps
### ValueDefiner
The training process generally contains minimizing or maximizing a certain value which is computed by the output of a training example and maybe some parameters like the target output of the training example. ValueDefiners are an abstraction of those "Value Computers". They are Neural Networks you can add to your NetworkContainer at any place you wish just like any NeuralNetwork. 
### ErrorDefiner
This is an implementation of a ValueDefiner using the mean squared error method to compute the value. It needs the target values for a training example in order to do so, so the target value array is "connected" to the target value output of the network container at the specified ports in the constructor.
