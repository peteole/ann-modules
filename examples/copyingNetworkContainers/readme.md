# Making network containers copyable
If you want to use multithreaded training, you need to make a copy of the NetwokContainer for each thread. 
However, you can change a lot about NetworkContainer (add Neural Networks and link them), so I decided to add the posibility of construcing them by giving a function that specifies how networks are added to the container and how they are linked.
## An example
This is an example function which constructs a container:
```cpp
void constructContainer(NetworkContainer &container) {

	//FullyConnectedNetwork with 3 layers with 5,4,2 neurons
	FullyConnectedNetwork *network1 = new FullyConnectedNetwork(new int[3] { 5,
			4, 3 }, 3);

	//FullyConnectedNetwork with 3 layers with 5,3,2 neurons
	FullyConnectedNetwork *network2 = new FullyConnectedNetwork(new int[3] { 5,
			5, 3 }, 3);

	//FullyConnectedNetwork with 2 layers with 4,3 neurons
	FullyConnectedNetwork *network3 = new FullyConnectedNetwork(new int[3] { 6,
			4,3 }, 3);

	//ValueDefiner with one channel for each output of the networkContainer
	ValueDefiner *errorDef = new ErrorDefiner(3, &container, 0);
	//let the networkContainer know it is about to work with all the networks we use
	container.addNeuralNetwork(network1);
	container.addNeuralNetwork(network2);
	container.addNeuralNetwork(network3);
	container.addNeuralNetwork(errorDef);

	//conntect the networks to each other
	container.makeNetworkChildOf(network1, &(container.in), 0);
	container.makeNetworkChildOf(network2, &(container.in), 5);
	container.makeNetworkChildOf(network3, network1, 0, 3, 0);
	container.makeNetworkChildOf(network3, network2, 0, 3, 3);
	container.makeNetworkChildOf(errorDef, network3, 0);
	container.makeNetworkChildOf(&(container.out), errorDef);

}
```
If you actually want to do it, it looks quite simple:
```cpp
NetworkContainer container(10, 3, 3, constructContainer);
```
## cloning the NetworkContainer
This can simply be done using the clone-Function. However, you need to cast it to the specific type of network you cloned:
```cpp
NetworkContainer *clone=dynamic_cast<NetworkContainer*>(container.clone());
```
This can be done with any Neural Network as the clone mehtod is inherited from the NeuralNetwork base class.
## copying the parameters of a cloned network
```cpp
copy->copyParameters(&container);
```
Note that the two networks really need to have an identical structure for using this method.
## adding the derivatives of a clone
```cpp
copy->addDerivatives(&container);
```
