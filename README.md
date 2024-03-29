# Strategic Calculation Optimization Research in Triangular Nim

## Overview

---

Triangular Nim is a two-player game where players take turns removing pieces from a set of chips arranged in an equilateral triangle. The game continues until the last remaining piece is taken, and the player who takes it loses. Each turn, players must remove at least one piece, and the pieces must be taken in a straight line without repetition.

The game of Triangular Nim is played on a triangle of side length k, forming k layers of chips. The larger the number of layers (k), the more challenging the strategic calculations become. Previous research has proposed winning strategies for Triangular Nim, and this project aims to optimize its algorithm through parallel programming methods to reduce computation time and lower overall computational costs

## Method

---

### [Backward Induction Approach](http://dspace.fcu.edu.tw/bitstream/2377/30046/1/AB%205-2.pdf)

**How to Parallel**

By employing the concepts of Producer and Consumer, parallelization is achieved. The Producer generates game states suitable for parallel computation and adds them to a task queue. The Consumer, in turn, retrieves tasks from the queue and computes the outcomes sequentially. This process is repeated until the computation is complete.

**How to Run Code**

```jsx
g++ backward.cc -o backward -O3
./backward
```

### [Retrograde Method](https://ir.nctu.edu.tw/bitstream/11536/15290/1/000299446100008.pdf)

**How to Parallel**

The goal is to identify independently calculable game states, where any two states A and B, after making legal moves, do not transform into each other. These states can be computed independently without affecting the results of each other.

**How to Run Code**

```jsx
g++ retro.cc -o retro -O3 -fopenmp
./retro
```

## Play With Computer

---

After the computation is complete, the winning strategy for Triangular Chess will be stored. Players have the option to engage in interactive matches against the computer, starting a game of Triangular Chess. However, since the computer has already stored the winning strategy, no matter how many times the game is played, the player will never win!

```jsx
g++ game.cpp -o game.out -std=c++11 -O3
./game.out
```

### Visualize the chessboard
```jsx
g++ visual.cc -o visual.out -std=c++11 -O3
./visual.out a_decimal_num(optional)
```
You need to type the numbers of the pieces you want to remove in a game. To know how to interact with the terminal, use `visual.out` to visualize a basic chessboard and also visualize your move if you turn your move into binary bit string, e.g. if you want to remove #1 and #2 pieces, your bit string is 0000000000000000000000000011 which equals 3.

## Makefile

You can use Makefile to compile. Just type `make` in terminal.

## Reference

- Yi Chang Shan, I-Chen Wu, Hung Hsuan Lin, Kuo Yuan Kao , “Solving Nine Layer Triangular Nim”, 2012
- S. Q. Bai and S. S. Lin, “On the study of 8 layer Triangular Nim,” in Proceedings of National Computer Symposium, 2009, pp. 60-71. 
- H. H. Lin, I. C. Wu, and Y. C. Shan, “Solving eight layer Triangular Nim,” in Proceedings of National Computer Symposium, 2009, pp. 200-204. 