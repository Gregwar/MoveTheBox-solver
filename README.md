Move The Box game solver
========================

This is a simple C program that can helps you in solving Move The Box
game boards.

Installation
------------

Download the source and simply type :

```
make
```

To compile the program `box`

Usage
-----

First, you have to write a map, to do this, simply edit a file in which
you should place spaces for blank and letters for boxes. The used symbol
to represent the box are not important, the only thing that matters is 
that each box has a different letter. Note that a map is a 7 columns and
10 rows matrix. For instance, this is the Level 4:

```








  YXY
  XYX
```

Then, run the box executable and pass it the map file and the number of 
tries as second argument:

```
Map loaded:
       
       
       
       
       
       
       
       
  YXY  
  XYX  
Success !
1) Switching col=4, line=9 with col=4, line=10
```

