# poker-bot ai

This part of the project is dedicated to making a monte-carlo/?self-play?/?deep-learning? poker "AI"???

Currently trying to implement an elementary neural net to classify NMIST number images... will work on different neural nets or even poker later on.

I don't know what I am doing...

# 

## 2021-05-01

Ok, so I've gone through [3B1B's video series on neural networks](https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi&index=1) and attempted to implement it into C++. This attempt at implementation into C++ can be seen in the `src/` folder. I've been trying to get it to do image classification on the MNIST images, but this yielded very hard to debug (because of course I need to debug! Theres no chance I got all this stuff correct the first time around!). Around the time of the first batch of debugging issues, I started using [Stanford's course notes on neural networks/machine learning](http://cs229.stanford.edu/syllabus-spring2021.html) to get another "angle of understanding" and highlight some dark spots in my knowledge.

## 2021-05-03

To simplify the debugging process, I'm trying to get a neural network to learn how to add 2 numbers together. This task should be able to be learned by a simple "neural network" with only 1 layer (and potentially no sigmoid/non-linear functions inside). By doing this, I'm turning my neural network into a glorified linear regression algorithm, but hopefully this just makes debugging easier.

### The simple problem (2021-05-04):

Add 2 numbers together. Will make 2 inputs, and one output. No hidden layers. This entire neural network can be represented by 1 linear expression: ![equation](https://latex.codecogs.com/gif.latex?h_{\theta}(x)&space;=&space;W_{\theta}x&space;&plus;&space;b_{\theta}). Where ![equation](https://latex.codecogs.com/gif.latex?W_{\theta}&space;\in&space;\mathbb{R}^{1\times&space;2}) represents all the weights that make up the network, and ![equation](https://latex.codecogs.com/gif.latex?b_{\theta}&space;\in&space;\mathbb{R}^{1}) represent all the biases. As can be seen, no non-linear ReLU or sigmoid function are found here yet, so this just acts as a simple linear model. This should be appropriate since all we are trying to do is predict the sum of 2 input variables - in this case, these 2 inputs are represented by ![equation](https://latex.codecogs.com/gif.latex?x&space;\in&space;\mathbb{R}^{2}).

Ideally, we would like to see ![equation](https://latex.codecogs.com/gif.latex?W_{\theta}&space;=&space;[1,1]) and ![equation](https://latex.codecogs.com/gif.latex?b_{\theta}&space;=&space;[0]) since this would make ![equation](https://latex.codecogs.com/gif.latex?h_{\theta}(x)&space;=&space;W_{\theta}x&space;&plus;&space;b_{\theta}&space;=&space;[1,1]x+[0]=x_1+x_2) which is what we are trying to find.

### Results:

For testing, I used 100 elements of training data. 1 element of the training data in this scenario looks something like: "0.62 + 0.17 = 0.79". For this case, 0.62 and 0.17 are the inputs, and 0.79 is the expected output of the model.

Initially, the weights and bias are set to random values. W = [[-0.997497, 0.127171]] and b = \[-0.613392].

Setting the "training batch size" to 10, after training we get the model's new weights and biases as: W = [[0.196435, 0.643688]] and b = \[0.469771]... not quite!

Setting the "training batch size" to 5, the model's new weights and biases are found to be: W = [[0.636033, 0.870602]] and b = \[0.366434]... better!

Finally, by setting the "training batch size" to 1, the model's new weights and biases are found to be: W = [[0.99684, 0.996662]] and b = \[0.00206637]... thats what we're looking for!

So, from initial inspection, the neural network (when dumbed down into a linear regression model) can learn the very simple task of adding 2 numbers!

Note: when training in "batches", it didn't work out to well... I wonder if this is just because of the lack of training data?

Trying with a training set of data with 500 entries with batch size of 10 again yields: W = [[0.952703, 0.971083]], and b = \[0.0351845]... ok, nice!

## 2021-05-04

Refactoring code to be consistent with Stanford's course notes. This means rewriting everything in "vectorized" notation (now taking partial derivatives of matrices and vectors instead of looping thru the vectors and doing the individual derivatives).