all: smallcharset bignumbers

smallcharset: smallcharset.pbm
	@echo -n Generating small character set...
	@xxd -s 10 -i smallcharset.pbm smallcharset.h
	@xxd -i smallcharset.map >>smallcharset.h
	@sed -i 's/unsigned/static const unsigned/g' smallcharset.h
	@echo done.

bignumbers: bignumbers.pbm
	@echo -n Generating big numbers...
	@xxd -s 10 -i bignumbers.pbm bignumbers.h
	@xxd -i bignumbers.map >>bignumbers.h
	@sed -i 's/unsigned/static const unsigned/g' bignumbers.h
	@echo done.
