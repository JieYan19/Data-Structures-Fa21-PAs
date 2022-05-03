# 3-1 Build 报告 

## 实现思路

### 多叉树结构和`MultiNode`结构体信息

本解法构建并维护了一个多叉树，为更好地组织数据结构，首先将多叉树由状态 A 转化为状态 B（以输入样例为例），以达到记录工作量最小化的作用：

```c++
    A                      B
    1                1
 /  |  \             |
3   2   4            3 — 2 — 4
|  /|\               |   |
5 8 6 7              5   8 — 6 — 7
```

采用`MultiNode`结构体记录多叉树中每一个结点的下列信息：

```c++
int parent;     //父节点，不存在记为-1
int firstChild; //第一个孩子结点，不存在记为-1
int leftBro;    //左兄弟结点，不存在记为-1
int rightBro;   //右兄弟结点，不存在记为-1
int height;     //高度，单结点高度为0
int size;       //子树规模，单结点子树规模为1
```

其中，`height`并非该结点的实际高度，而是该结点`rightBro`的`height`和(`firstChild`的`height`+1)中的最大值。这样记录的好处是，当移动某结点后进行高度维护时，无需重新遍历全部孩子结点来寻找最大实际高度，而是仅需重新比较需更新高度结点的`rightBro`的`height`和(`firstChild`的`height`+1)中的最大值，大大缩减了复杂度。具体更新高度的方法会在下一节中详细说明。

### `MultiTree`类的方法和总体运行流程

`MultiTree`类用于存放构建多叉树的方法和若干操作接口，具体接口如下：

```c++
int initHeight(int curNode = 1){}; //初始化height
int initSize(int curNode = 1){}; //初始化size
int find(int *path, int pathLen, int num = 0, int data = 1){}; //查找结点
void move(int *pathDep, int *pathDes, int childPos, int pathDepLen, int pathDesLen){}; //子树移动
void updateHeight(int curNode){}; //更新高度
void updateSize(int moveNode, int newParent){}; //更新子树规模
int showHeight(int *path, int pathLen){}; //查询高度
int showSize(int *path, int pathLen){}; //查询子树规模
```

对于一套输入而言，首先构造出多叉树并初始化`height`和`size`。若进行子树移动操作，则调用`move`函数，在`move`函数中，会调用`find`函数确定结点编号，并在移动完成后，调用`updateHeight`函数和`updateSize`函数维护高度和子树规模。若进行查询操作，则根据查询内容分别调用相应的查询函数。

### `MultiTree`类实现细节的重点和难点

**① 初始化**：在初始化时，`parent`、`firstChild`、`leftBro`和`rightBro`都能伴随输入赋值。`height`的计算由`initHeight`函数完成，若当前结点既无孩子又无右兄弟结点，则`height=0`；若当前结点有孩子结点，则`height=firstChild.height+1`；若当前结点有右兄弟结点，则`height=rightBro.height`；若当前结点既有孩子结点又有右兄弟结点，则`height=max(rightBro.height,firstChild.height+1)`。`size`的计算由`initSize`函数完成，若当前结点无孩子结点，则`size=1`；有孩子结点则`size=所有孩子结点的size+1`。在具体实现方面，`initHeight`函数和`initSize`函数均由递归和备忘录算法完成。

**②子树移动**：在子树的移动中，需要更改结点连接信息和更新高度与子树规模。对于前者，只需更改源子树根结点的`parent`、`leftBro`和`rightBro`，以及其原本的和新的`parent`、`leftBro`和`rightBro`结点的参数信息。对于后者，则需要在完成移动后调用`updateHeight`函数和`updateSize`函数来实现。整个过程中需要特别注意判断结点的`parent`、`leftBro`和`rightBro`是否存在，`move`方法的主要过程在于分类讨论。

**③子树规模维护和查询**：`size`的维护较简单，原父结点一路向上取父结点全部减去源子树的`size`，新父结点一路向上取父结点全部加上源子树的`size`。查询子树规模时，返回查询结点的`size`。

**④高度维护和查询**：`height`的定义、初始化和维护是整个题目中最难思考的部分，`updateHeight`函数主要用于每次`move`操作后维护被波及的结点的正确`height`。维护的对象包括当前结点的全部`leftBro`和`parent`，蛇形向上维护，以`leftBro`优先。维护的方式是重新计算维护对象的`height`，采取`initHeight`函数中类似的方式求得。查询高度时，返回查询结点的`firstChild.height+1`。

## 错误修正

本题经过的错误修正较少，仅有两点：

### 第一点

最初没有想清楚在哪里更新高度以及旧、新两次高度更新的统一写法，导致`updateHeight`函数的实现较为混乱，造成WA。

### 第二点

在`move`函数中，重新赋值移动结点目标位置周围结点的参数时，进行了

```c++
node[cur].leftBro = depRoot;
node[depRoot].leftBro = node[cur].leftBro;
```

这种操作。错误很显然，在于过早地更改了`node[cur].leftBro`的值，使得`node[depRoot].leftBro = depRoot`，造成WA和TLE。

## 复杂度估计

时间复杂度主要在更新高度上，更新高度需遍历移动结点的全部左上方向的结点。空间复杂度则单纯依赖于输入结点的规模。

时间复杂度：O(m(∑rank+1)) = O∑cost+m)

空间复杂度：O(n)