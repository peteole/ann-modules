# ann-modules
A modular system to create and train your own custom neural networks
## installation
Just copy the repository to your local machine
## The classes used
The idea is to create a set of components you can connect to each other as you wish.
### PlugOut, PlugIn
You can imagine these classes just as a pair of mechanical plugs you can connect to each other: 
The PlugOut provides access to the output of a neural network and the PlugIn can recieve input if connected to a PlugOut.
### NeuralNetwork
This is an abstraction of a neural network. It is a child of PlugIn and PlugOut as it needs to recieve input and provide an output.
