all:
	bison -d murasaki.y
	flex murasaki.flex
	gcc murasaki.tab.c lex.yy.c -ll

clean:
	rm lex.yy.c murasaki.tab.*
