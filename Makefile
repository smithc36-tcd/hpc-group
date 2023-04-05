cc=pandoc

assignment_one:
	pandoc -s -V geometry:margin=1in -o assignment_1.pdf ass_1.md
