{\rtf1\ansi\ansicpg950\cocoartf1347\cocoasubrtf570
{\fonttbl\f0\froman\fcharset0 Times-Roman;\f1\fnil\fcharset136 STSongti-TC-Regular;}
{\colortbl;\red255\green255\blue255;}
{\*\listtable{\list\listtemplateid1\listhybrid{\listlevel\levelnfc23\levelnfcn23\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace360\levelindent0{\*\levelmarker \{disc\}}{\leveltext\leveltemplateid1\'01\uc0\u8226 ;}{\levelnumbers;}\fi-360\li720\lin720 }{\listname ;}\listid1}}
{\*\listoverridetable{\listoverride\listid1\listoverridecount0\ls1}}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\pardirnatural

\f0\fs40 \cf0 Compiler hw2 readme\
\pard\pardeftab720\pardirnatural

\fs24 \cf0 Topic: LL(1)parser\
Environment: Linux on MacOS\
Input: grammar.txt main.c\
Output: set1.txt LL_TABLE.txt tree1.txt\
Description:\

\f1 \'b0\'b5\'a5\'58\'ad\'6e\'a6\'73
\f0 grammar token 
\f1 \'aa\'ba
\f0 structure
\f1 \'a1\'41
\f0 grammar 
\f1 \'ac\'4f\'a5\'ce\'b4\'ab\'a6\'e6\'a8\'d3\'b0\'b5\'a4\'c0\'a5\'aa\'a5\'6b\'c3\'e4\'a1\'41\'a7\'51\'a5\'ce\'a4\'6a\'a4\'70\'bc\'67\'a4\'c0\'a7\'4f\'a6\'73\'a4\'4a
\f0 Vnonterminal,Vterminal
\f1 \'a1\'43
\f0 \
Find_first(): 
\f1 \'b1\'a1\'aa\'70\'a4\'40\'a1\'47\'a5\'6b\'c3\'e4\'b2\'c4\'a4\'40\'ad\'d3\'ac\'4f
\f0 terminal
\f1 \'a1\'41\'b7\'6a\'b4\'4d
\f0 Vt
\f1 \'a1\'41\'a7\'e4\'a8\'ec\'ab\'4b\'a6\'a8\'a5\'df\'a6\'73\'a4\'4a
\f0 first\
		   
\f1 \'b1\'a1\'aa\'70\'a4\'47\'a1\'47\'a5\'6b\'c3\'e4\'b2\'c4\'a4\'40\'ad\'d3\'ac\'4f
\f0 nonterminal
\f1 \'a1\'41\'a7\'e4\'a8\'ec\'b9\'ef\'a6\'4c\'b1\'6f\'a4\'55\'a4\'40\'ad\'d3
\f0 symbol
\f1 \'a1\'41\'ad\'ab\'b7\'73\'bb\'bc\'b0\'6a\'c0\'cb\'c5\'e7\'a1\'43
\f0 \
\
Find_follow(): 
\f1 \'b1\'a1\'aa\'70\'a4\'40\'a1\'47
\f0 follow
\f1 \'ab\'e1\'ad\'b1\'ac\'b0
\f0 terminal
\f1 \'a5\'42\'b5\'4c
\f0 epsilon\
			 
\f1 \'b1\'a1\'aa\'70\'a4\'47\'a1\'47
\f0 follow
\f1 \'ab\'e1\'a8\'53\'a6\'b3\'aa\'46\'a6\'e8
\f0  
\f1 \'a9\'d2\'a5\'48\'ac\'b0\'a5\'aa\'c3\'e4\'aa\'ba
\f0 follow
\f1 \'b6\'7d\'a9\'6c\'bb\'bc\'b0\'6a\'b7\'6a\'b4\'4d
\f0 \
			 
\f1 \'b1\'a1\'aa\'70\'a4\'54\'a1\'47
\f0 follow
\f1 \'ab\'e1\'ac\'b0
\f0 nonterminal or epsilon\

\f1 \'b7\'ed
\f0 follow
\f1 \'ab\'e1\'a6\'b3
\f0 nonterminal
\f1 \'a1\'41\'a7\'e4
\f0 nonterminal
\f1 \'aa\'ba
\f0 first \

\f1 \'b7\'ed
\f0 follow
\f1 \'ab\'e1\'a6\'b3
\f0 epsilon 1.
\f1 \'ab\'e1\'ad\'b1\'b1\'b5
\f0 terminal 2.
\f1 \'ab\'e1\'ad\'b1\'a8\'53\'aa\'46\'a6\'e8
\f0  3.
\f1 \'ab\'e1\'ad\'b1\'b1\'b5
\f0 nonterminal\
\pard\tx220\tx720\pardeftab720\li720\fi-720\pardirnatural
\ls1\ilvl0\cf0 {\listtext	\'95	}
\f1 \'bf\'e9\'a4\'4a\'a6\'dc
\f0 follow set\
{\listtext	\'95	}
\f1 \'ad\'ab\'b7\'73\'b0\'b5\'bb\'bc\'b0\'6a
\f0 \
{\listtext	\'95	}
\f1 \'a7\'e4
\f0 nonterminal
\f1 \'aa\'ba
\f0 first 
\f1 \'bf\'e9\'a4\'4a\'a6\'dc
\f0 follow set\
\pard\pardeftab720\pardirnatural
\cf0 \
LL_TABLE\

\f1 \'a4\'f1\'b9\'ef
\f0 first set 
\f1 \'bb\'50
\f0  follow set 
\f1 \'a4\'a4\'aa\'ba
\f0 terminal
\f1 \'a1\'41\'bf\'e9\'a5\'58\'a6\'a8\'a5\'6b\'c3\'e4\'aa\'ba
\f0  production\
\
tree\

\f1 \'a5\'fd\'b0\'b5\'a4\'40\'ad\'d3
\f0 stack 
\f1 \'aa\'ba
\f0  constructor
\f1 \'a1\'41\'b8\'cc\'ad\'b1\'b0\'b5
\f0 stack
\f1 \'aa\'ba
\f0 push 
\f1 \'bb\'50
\f0 pop\

\f1 \'b6\'c7\'a4\'4a
\f0 lexer
\f1 \'b5\'b2\'aa\'47\'aa\'ba\'a6\'73\'a6\'a8
\f0 structure 
\f1 \'b6\'c7\'a4\'4a\'a1\'41\'a6\'b3\'ac\'db\'b2\'c5\'b4\'4e\'ab\'d8\'a5\'df
\f0 tree\
}