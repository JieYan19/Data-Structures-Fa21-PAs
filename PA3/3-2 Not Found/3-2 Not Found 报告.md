# 3-2 Not Found 报告

## 实现思路

本题重点在于对时间和空间复杂度的把控，要不断优化算法；相对而言，此题代码实现较容易，不涉及复杂的需要分类讨论的逻辑。大体思路是，构建一棵包含`(1<<25)-1`个结点的字典树，由`Bitmap`压位存储结点是否存在的信息，这样大约只占用`4M`的内存。而后，将输入串中每一个长度不大于`24`的短串映射到字典树上的点，这样就能让所有存在的字符串对应的结点都被标记，具体映射方式在下文中有详细介绍。最后，遍历字典树，找到序号最小的没有被标记的结点，即为字典序最小的未出现串。

具体过程如下：

首先，由于输入串`A`的长度`≤ 16,777,216 = 2^24`，由于长度为`24`的短串有`2^24`种，因此，输入串`A`中一定至少有一个长度为`24`的短串没有出现过。由于我们只需输出字典序最小的短串，所以我们无需考虑长度大于`24`的串。

然后，我们需要在字典树中标记输入串中所有长度不大于`24`的短串，这里需要一定简化。以`1101`为例，我们可以发现在这个串中有`(1101、110、11、1)、(101、10、1)、(01、1)、(1)`这些字串出现。若将其映射到字典树中，则会发现`1101`的路径中包含`110、11和1`的路径，因此无需重复标记`110、11和1`。同理，前面的括号组中只有第一个串的路径需要被标记。

接下来需要找到需要标记的结点的编号，我们可以在下图的完全二叉字典树中找到：

```c++
                      1
              /                \
           2                      3
       /       \             /         \
     4          5           6           7
   /   \      /   \       /   \       /   \
  8     9    10    11    12    13    14    15
 / \   / \   / \   / \   / \   / \   / \   / \
16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
```

- `1101`经过`3、7、14、29`这`4`个点
- `101`经过`3、6、13`这`3`个点
- `01`经过`2、5`这`2`个点
- `1`经过`3`这`1`个点

如果我们每经过一个点及进行一次标记，将会导致效率一定程度上降低，因此，我们应该直接定位到`29、13、5、3`这些点，再在最后统一计算需要标记的父结点并标记。可以发现，二进制串到结点编号会有这样的映射公式：`num = num + (1 << i)`。然后，我们再将映射得到的结点编号存在`Bitmap`类中。`Bitmap`类包含如下成员变量和方法：

```c++
unsigned char *M; //比特图所存放的空间M[]，char为一个字节
int N; //容量为N*sizeof(char)*8比特
init(int n){} //初始化
void set(int k){} //插入
bool test(int k){} //判断
```

由于我们只关心结点有没有标记这一个信息，因此使用`Bitmap`类可以将`1<<25`量级的结点存储在`1<<22`量级的`char`数组中。

如果输入串的长度大于`24`，还需要利用`num = (num % (1 << 23)) * (1 << 1) + ((int)curChar - 48)`依次向后挪动标记。对于最后剩余的长度`≤24`的尾串，需要调用`resWrite`函数逐次去头并写入`bitmap`中。

最后，遍历字典树将各标记结点的父节点利用`num = num >> 1`全部标记，并再次遍历字典树，找到序号最小的没有被标记的结点，转化为二进制串输出。

## 错误修正

无。

## 复杂度估计

时间复杂度：O(n+C)

空间复杂度：O(1<<22)