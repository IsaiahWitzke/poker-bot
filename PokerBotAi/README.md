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

### Results (2021-05-04/05):

For testing, I used 100 elements of training data. 1 element of the training data in this scenario looks something like: "0.62 + 0.17 = 0.79". For this case, 0.62 and 0.17 are the inputs, and 0.79 is the expected output of the model.

Initially, the weights and bias are set to random values. W = [[-0.997497, 0.127171]] and b = \[-0.613392].

Setting the "training batch size" to 10, after training we get the model's new weights and biases as: W = [[0.196435, 0.643688]] and b = \[0.469771]... not really what I was looking for, but the error ([MAPE](https://en.wikipedia.org/wiki/Mean_absolute_percentage_error)) of this model is only 13.54%.

Setting the "training batch size" to 5, the model's new weights and biases are found to be: W = [[0.636033, 0.870602]] and b = \[0.366434]... better! The error is now 7.47%.

Finally, by setting the "training batch size" to 1, the model's new weights and biases are found to be: W = [[0.99684, 0.996662]] and b = \[0.00206637]... thats what we're looking for! Error is now 3.03%! 

So, from initial inspection, the neural network (when dumbed down into a linear regression model) can learn the very simple task of adding 2 numbers!

Note: when training in "batches", it didn't work out too well... I wonder if this is just because of the lack of training data?

Trying with a training set of data with 500 entries with batch size of 10 again yields: W = [[0.952703, 0.971083]], and b = \[0.0351845]... ok, nice! Error is now 1.74%!

## 2021-05-04

Refactoring code to be consistent with Stanford's course notes. This means rewriting everything in "vectorized" notation (now taking partial derivatives of matrices and vectors instead of looping thru the vectors and doing the individual derivatives).

## 2021-05-05

Now to test if my implementation works when it isn't dumbed down into a linear regression model. To do this, I will use the same problem of training the model to add 2 numbers together, but will just add in a new hidden layer with 2 neurons. This means that the ReLU will actually come into effect. Note: still using training set of size 500 with training batch size of 10.

Before actually doing this, there are a couple different "optimal solutions" that I think the neural network should converge towards:
- Option 1: the weights from the first layer is just the identity matrix (or it's transpose), and weights connecting the second layer to the outputs is just [[1,1]]. This would mean that the neural network would function almost identically to the linear regression model produced yesterday.
- Option 2: the adding is done in the first layer and the answer is just propagated 1 layer more into the output. This would mean that the first set of weights would look something like W^\[1] = [[1,1],[0,0]] and the second layer of weights like: W^\[2] = [[1,0]].
- Option 3: Theres defiantly some other optimal solutions that could be found... but I don't want to put too much brain power into predictions at the moment.

### Results:

I have no clue what just happened.... this isn't anything close to what I expected the trained model's weights and biases to look like....

```json
{
    "layers" : 3,
    "scalarFuncsCompressFactor" : 1,
    "layer_0" : {
        "layer" : 0,
        "neurons" : 2,
        "weights" : 
        [
            [-1.23075, -0.00211784],
            [0.302324, 1.28141]
        ],
        "biases" : [0.913344, -0.302128]
    },
    "layer_1" : {
        "layer" : 1,
        "neurons" : 2,
        "weights" : 
        [
            [-0.715883, 0.743782]
        ],
        "biases" : [0.950913]
    },
    "layer_2" : {
        "layer" : 2,
        "neurons" : 1
    }
}
```

However, the MAPE error is only 10.08%... interesting.

As I figured out yesterday, as I decreased the training batch size, the inaccuracy of the model decreased. Lets see what happens if I turn the training batch size to 5: 

With batch size = 5, inaccuracy decreases to 9.07%. Not too bad. The trained model's weights and biases look like:

```json
{
    "layers" : 3,
    "scalarFuncsCompressFactor" : 1,
    "layer_0" : {
        "layer" : 0,
        "neurons" : 2,
        "weights" : 
        [
            [-1.25318, -0.442124],
            [0.56567, 1.2925]
        ],
        "biases" : [1.2435, -0.525241]
    },
    "layer_1" : {
        "layer" : 1,
        "neurons" : 2,
        "weights" : 
        [
            [-0.699033, 0.667628]
        ],
        "biases" : [0.923608]
    },
    "layer_2" : {
        "layer" : 2,
        "neurons" : 1
    }
}
```

Which actually haven't changed very much from previous.

Lets try setting the batch size to 1... 

Inaccuracy skyrockets to 30%!?!?!?! New weights and biases look like:

```json
{
    "layers" : 3,
    "scalarFuncsCompressFactor" : 1,
    "layer_0" : {
        "layer" : 0,
        "neurons" : 2,
        "weights" : 
        [
            [-2.31734, -1.44809],
            [-0.269041, 0.705985]
        ],
        "biases" : [-0.386383, -0.932682]
    },
    "layer_1" : {
        "layer" : 1,
        "neurons" : 2,
        "weights" : 
        [
            [0.659918, -1.41288]
        ],
        "biases" : [0.998339]
    },
    "layer_2" : {
        "layer" : 2,
        "neurons" : 1
    }
}
```
That makes no sense. Maybe this is what is meant by "overfitting"? When in doubt, through more data at the network? Increasing the training set to a size of 1000, yields the following results:

- Batch size = 1: Error = 30.57%
- Batch size = 4: Error = 30.18%
- Batch size = 5: Error = 9.14%
- Batch size = 10: Error = 21.07%
- Batch size = 100: Error = 19.15%

Nothing makes sense :(

## 2021-05-05 10:00PM

Things make sense again. After doing some big time debugging, I realized that the "adjustments steps" that the model undergoes while training is too large - especially when the model is close to the optimal solution. This means, what it will realize in which direction that the weights/biases need to be tuned to improve accuracy, but will actually just overcompensate! Once overcompensation occurs, the next training iteration can overcompensate AGAIN but in the opposite direction. This out of control oscillation can lead to wild results (as seen above), especially when the model begins overcompensating itself so that many neurons' activations are cut to 0 by the relu.

In short: by scaling the "nudges" to the weights and biases by a factor of 0.5 while training fixes this "overcompensation" effect. Here are the new results:

- Batch size = 1: Error = 0.17%
- Batch size = 4: Error = 0.0010%
- Batch size = 5: Error = 0.0033%
- Batch size = 10: Error = 0.81%
- Batch size = 100: Error = 16.73%

For the batch size = 10, this is the model's trained weights and biases:

```json
{
    "layers" : 3,
    "scalarFuncsCompressFactor" : 1,
    "layer_0" : {
        "layer" : 0,
        "neurons" : 2,
        "weights" : 
        [
            [-0.0369923, 0.465357],
            [0.947378, 0.922853]
        ],
        "biases" : [0.333466, -0.0203885]
    },
    "layer_1" : {
        "layer" : 1,
        "neurons" : 2,
        "weights" : 
        [
            [0.0702497, 1.05017]
        ],
        "biases" : [0.00733153]
    },
    "layer_2" : {
        "layer" : 2,
        "neurons" : 1
    }
}
```

It seems like it found an optimal set of weights and biases that resemble the "Option 2" prediction I made aerier today.
