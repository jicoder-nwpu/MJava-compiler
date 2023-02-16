# MJava编译器词法及语法分析功能的设计与实现

本项目为《编译原理》课程设计，MJava是一种类Java的简易编程语言。在给定的词法规则及语法规则下，利用DFA完成该编译器词法分析功能的设计与实现，基于上下文无关文法设计出抽象语法树，采用递归下降算法实现该编译器的语法分析功能。

##### MJava词法分析的DFA

![](images/wps1.jpg)

##### MJava语法分析抽象语法树结构

###### Goal-> MainClass { ClassDeclaration } EOF

![img](images/wps2.jpg) 

###### MainClass->"class" Identifier "{" "public" "static" "void" "main" "(" "String" "[" "]" Identifier ")" "{" Statement "}" "}"

![img](images/wps3.jpg) 

###### ClassDeclaration->"class" Identifier [ "extends" Identifier ] "{" { VarDeclaration } { MethodDeclaration } "}"

![img](images/wps4.jpg) 

###### VarDeclaration ->Type Identifier ";"

![img](images/wps5.jpg) 

###### MethodDeclaration->"public" Type Identifier "(" [ Type Identifier { "," Type Identifier } ] ")" "{" { VarDeclaration } { Statement } "return" Expression ";" "}"

![img](images/wps6.jpg) 

![img](images/wps7.jpg) 

###### Type->"int" "[" "]"|"boolean"|"int"| Identifier

![img](images/wps8.jpg) 

###### Statement->"{" { Statement } "}"

![img](images/wps9.jpg) 

###### Statement->"if" "(" Expression ")" Statement "else" Statement 

![img](images/wps10.jpg) 

###### Statement->"while" "(" Expression ")" Statement 

![img](images/wps11.jpg) 

###### Statement->"System.out.println" "(" Expression ")" ";"

![img](images/wps12.jpg) 

###### Statement-> Identifier "=" Expression ";"

###### Statement-> Identifier "[" Expression "]" "=" Expression ";"

![img](images/wps13.jpg) 

###### Expression-> Expression "[" Expression "]"

![img](images/wps14.jpg) 

###### Expression-> Expression  ( "&&" | "<" | "+" | "-" | "" ) Expression 

![img](images/wps15.jpg) 

###### Expression-> Expression "[" Expression "]"

![img](images/wps16.jpg) 

###### Expression-> Expression "." "length"

![img](images/wps17.jpg) 

###### Expression-> Expression "." Identifier "(" [ Expression { "," Expression } ] ")"

![img](images/wps18.jpg) 

###### Expression-> IntegerLiteral |"true"|"false"| Identifier |"this"

![img](images/wps19.jpg) 

 

###### Expression->"new" "int" "[" Expression "]"

![img](images/wps20.jpg) 

###### Expression->"new" Identifier "(" ")"

![img](images/wps21.jpg) 

###### Expression-> "!" Expression 

![img](images/wps22.jpg) 

###### Expression->"(" Expression ")"

![img](images/wps23.jpg) 
