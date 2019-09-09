# CUIMinesweeperWithSpreadingZeros
This program is a simple CUI minesweeper in C. (with spreading zeros)

【More details】

■English, 日本語
https://ka373.blogspot.com/2019/09/blog-post_9.html

■한국어
https://ka373.tistory.com/30

I. Purpose of the game
You can clear the game by stepping on all the ground except for the mines on the board.

II. Progress of the game
There is a board, and the game is played by stepping on it, marking a mine or "?".

__________________________________________
[Some production processes]


0. Structure of the game
Repeat 1) ~ 4) while the game continues
 1) Input block position and type.
 2) Enter the location and type of the block in the game progress function.
 3) Determine whether the player lived or died.
 4) Print the progress (board).


1. Create boards
Since the game is played on board, we need boards.
 1) the visible board I'm exploring(board_visible)
 2) the answer board (board_kotae).


2. Configure the answer board (board_kotae)

2.1. Build and deploy mines
Generate as many random numbers as the number of blocks on the board, and place mines on the board according to the random numbers.

2.2. Number setting
Searching through the array, assigning numbers by counting the number of mines in the block around them.


3. Create a game progress function (process_game)

3.1. If the input location is already visited, it is invalidated.

3.2. If the user stepped on a new block
Refer to the answer board and enter the value in the visible board.
Here the value entered in the visible board is used to print the board.

3.2.1. User stepped on: A number between 1 and 8
Enter the number on the visible board.

3.2.2. User stepped on: Landmine

Enter the number corresponding to the visible board.
Returns a value indicating gameover (in my case, is_live = 0;)

3.2.3. User stepped on: 0 (If there are no zeros around the block)
If the number of the selected block is 0, 0 is spread out, and the surrounding 0 is also treated as stepped on.

I think this part is harder to implement than other parts. therefore, If you want to implement minesweeper on your own, please try to solve it yourself before referring to this section.

In my case, I implemented it through recursive function as follows.

If the step is 0, the process is performed recursively in the upper left, upper, upper right, lower left, lower, and lower right directions.

It's a bit of a hobby development, so you can't be sure it's an optimized algorithm. However, It doesn't seem to be a big problem just for the performance we need.

I hope someone reading this article also thinks about other good algorithms.
