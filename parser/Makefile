all:
	bison -d murasaki.y
	flex murasaki.flex
	g++ lex.yy.c -ll

clean:
	rm lex.yy.c murasaki.tab.*
