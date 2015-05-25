Compiler hw2 readme
Topic: LL(1)parser
Environment: Linux on MacOS
Input: grammar.txt main.c
Output: set1.txt LL_TABLE.txt tree1.txt
Description:
做出要存grammar token 的structure，grammar 是用換行來做分左右邊，利用大小寫分別存入Vnonterminal,Vterminal。
Find_first(): 情況一：右邊第一個是terminal，搜尋Vt，找到便成立存入first
		   情況二：右邊第一個是nonterminal，找到對印得下一個symbol，重新遞迴檢驗。

Find_follow(): 情況一：follow後面為terminal且無epsilon
			 情況二：follow後沒有東西 所以為左邊的follow開始遞迴搜尋
			 情況三：follow後為nonterminal or epsilon
當follow後有nonterminal，找nonterminal的first 
當follow後有epsilon 1.後面接terminal 2.後面沒東西 3.後面接nonterminal
1.	輸入至follow set
2.	重新做遞迴
3.	找nonterminal的first 輸入至follow set

LL_TABLE
比對first set 與 follow set 中的terminal，輸出成右邊的 production

tree
先做一個stack 的 constructor，裡面做stack的push 與pop
傳入lexer結果的存成structure 傳入，有相符就建立tree
