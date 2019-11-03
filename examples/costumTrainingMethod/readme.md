# Creating your own training methods
The updateParameters- Function now accepts a pointer to a function which is called in order to update a network's parameters. The function gets a reference to the derivative, a reference to the current value of the parameter and a pointer of the char type to a place where costum values for each parameter can be stored (for example the last derivative of the parameter). 
## create the costum storage space for each parameter
This is done specifying the size of the costum storage for each parameter. For example, if you want to store two doubles, it would be 2*8byte:
```cpp
container.createBufferStorage(16);
```
## create your training method
Note that you will probably need to convert your raw parameter storage to the datatype you uses with a reinterpret_cast. This is an example implementation for gradient descent using momentum:
```cpp
#define a 0.0001
#define beta 0.9
void momentumDescent(double &derivative, double &oldValue,char* parameters){
	double &lastUpdate=*reinterpret_cast<double*>(parameters);
	double update=a*(derivative+beta*lastUpdate);
	oldValue-=update;
	lastUpdate=update;
}
```
## call the training method
You must specify your training method as a parameter. For example, if it is called "momentumDescent", it would be:
```cpp
container.updateParameters(momentumDescent);
```
Note that the addDerivatives-Method and the evaluation of training examples is independent from this.
