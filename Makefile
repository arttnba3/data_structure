OUTPUT_DIR := $(shell pwd)/out
LIB_DIR := $(shell pwd)

all: env link

link: user.o shop.o info.o scanner.o main.o
	cd "$(OUTPUT_DIR)" && g++ $? -o OnlineShop -lpthread -lcrypt

clean:
	rm -rf "$(OUTPUT_DIR)"

env:
	if [ ! -d "$(OUTPUT_DIR)" ]; then \
		mkdir "$(OUTPUT_DIR)" ; \
	fi ;

main.o: main.c
	gcc -c $? -o $@ -I "${LIB_DIR}"
	mv $@ '$(OUTPUT_DIR)'

shop.o: shop/shop.c
	gcc -c $? -o $@ -I "${LIB_DIR}"
	mv $@ '$(OUTPUT_DIR)'

user.o: user/user.c
	gcc -c $? -o $@ -I "${LIB_DIR}"
	mv $@ '$(OUTPUT_DIR)'

scanner.o: scanner/scanner.c
	gcc -c $? -o $@ -I "${LIB_DIR}"
	mv $@ '$(OUTPUT_DIR)'

info.o: info/info.c
	gcc -c $? -o $@ -I "${LIB_DIR}"
	mv $@ '$(OUTPUT_DIR)'
