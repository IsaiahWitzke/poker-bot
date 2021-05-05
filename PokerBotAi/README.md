# poker-bot ai

This part of the project is dedicated to making a monte-carlo/?self-play?/?deep-learning? poker "AI"???

Currently trying to implement an elementary neural net to classify NMIST number images... will work on different neural nets or even poker later on.

I don't know what I am doing...

# 

## 2021-05-01

Ok, so I've gone through [3B1B's video series on neural networks](https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi&index=1) and attempted to implement it into C++. This attempt at implementation into C++ can be seen in the `src/` folder. I've been trying to get it to do image classification on the MNIST images, but this yielded very hard to debug (because of course I need to debug! Theres no chance I got all this stuff correct the first time around!). Around the first debugging issues, I started using [Stanford's course notes on neural networks/machine learning](http://cs229.stanford.edu/syllabus-spring2021.html) to get another "angle of understanding" and highlight some dark spots in my knowledge. 

## 2021-05-03

To simplify the debugging process, I'm trying to get a neural network to learn how to add 2 numbers together. This task should be able to be learned by a simple "neural network" with only 1 layer (and potentially no sigmoid/non-linear functions inside). By doing this, I'm turning my neural network into a glorified linear regression algorithm, but hopefully this just makes debugging easier.

### The simple problem (2021-05-04):

Add 2 numbers together. Will make 2 inputs, and one output. No hidden layers. This entire neural network can be represented by 1 linear expression: ![equation](https://latex.codecogs.com/gif.latex?h_{\theta}(x)&space;=&space;W_{\theta}x&space;&plus;&space;b_{\theta}). Where ![equation](https://latex.codecogs.com/gif.latex?W_{\theta}&space;\in&space;\mathbb{R}^{1\times&space;2}) represents all the weights that make up the network, and ![equation](https://latex.codecogs.com/gif.latex?b_{\theta}&space;\in&space;\mathbb{R}^{2}) represent all the biases. As can be seen, no non-linear ReLU or sigmoid function are found here yet, so this just acts as a simple linear model. This should be appropriate since all we are trying to do is predict the sum of 2 input variables - in this case, these 2 inputs are represented by ![equation](https://latex.codecogs.com/gif.latex?x&space;\in&space;\mathbb{R}^{2}).

Ideally, we would like to see ![equation](https://latex.codecogs.com/gif.latex?W_{\theta}&space;=&space;[1,1]) and ![equation](https://latex.codecogs.com/gif.latex?b_{\theta}&space;=&space;[0,0]^T) since this would make ![equation](https://latex.codecogs.com/gif.latex?h_{\theta}(x)&space;=&space;W_{\theta}x&space;&plus;&space;b_{\theta}&space;=&space;as)

## 2021-05-04

Refactoring code to be consistent with Stanford's course notes. This means rewriting everything in "vectorized" notation (now taking partial derivatives of matrices and vectors instead of looping thru the vectors and doing the individual derivatives).